import torch




import torch.nn as nn
import torch.nn.functional as F


class MLP(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(10, 15)
        self.fc2 = nn.Linear(15, 20)
        self.fc3 = nn.Linear(20, 5)

    def forward(self, x):
        x = torch.flatten(x, 1) # flatten all dimensions except batch
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x



class Linear:
    def __init__(self, name, input_size, output_size, weights, biases, dtype):
        self.name = name
        self.input_size = input_size
        self.output_size = output_size
        self.weights = weights
        self.biases = biases
        if dtype == torch.float32:
            self.dtype = "DTYPE_F32"
        elif dtype == torch.int8:
            self.dtype = "DTYPE_I8"
        elif dtype == torch.int32:
            self.dtype = "DTYPE_I32"
        else:
            raise ValueError("Unsupported dtype: " + str(dtype))

    def getWeights(self):
        weights = str(self.weights.flatten().tolist()).replace('[', '{').replace(']', '}')
        biases = str(self.biases.flatten().tolist()).replace('[', '{').replace(']', '}')
        return f"""
const float {self.name}_weight_data[{self.input_size * self.output_size}] = { weights };
const float {self.name}_bias_data[{self.output_size}] = { biases };"""

    def getCDeclare(self):
        # return f"NN_Linear({self.input_size}, {self.output_size});"
        return f"""float {self.name}_out_data[{self.input_size}] = {{0}};
Tensor {self.name}_out = NN_tensor(2, (size_t[]){{ 1, {self.output_size} }}, {self.dtype}, (float *){self.name}_out_data);
Tensor {self.name}_weight = NN_tensor(2, (size_t[]){{ {self.input_size}, {self.output_size} }}, {self.dtype}, (float *){self.name}_weight_data);
Tensor {self.name}_bias = NN_tensor(2, (size_t[]){{ 1, {self.output_size} }}, {self.dtype}, (float *){self.name}_bias_data);"""

    def getCForward(self, last_layer):
        # return f"NN_forward_linear({self.name}_out, {self.name}_in, {self.name}_weight, {self.name}_bias);"
        return f"NN_Linear_F32(&{self.name}_out, &{last_layer}, &{self.name}_weight, &{self.name}_bias);"




class TorchConverter:
    def __init__(self, model: nn.Module, input_names: list, output_names: list):
        self.model = model
        self.input_names = input_names
        self.output_names = output_names
        
        self.impl: list[Linear] = []

        # (1, 15) @ (20, 15).T + (1, 20) -> (1, 20)

        for layer_name, module in model.named_modules():
            print("Find network:", layer_name, type(module))

            if type(module) == torch.nn.Linear:
                print("Found Linear layer")
                layer = Linear(layer_name, module.in_features, module.out_features, module.weight, module.bias, module.weight.dtype)
                
                self.impl.append(layer)

    def save(self, filename):

        # write to file
        with open(filename, "w") as f:
            f.write("#include <stdio.h>\n")
            f.write("#include <stdint.h>\n")
            f.write("#include <stdlib.h>\n")
            f.write("#include <string.h>\n")
            f.write("#include \"nn.h\"\n")
            for layer in self.impl:
                f.write(layer.getWeights())
                f.write("\n")
            f.write("\n")

            
            f.write("Tensor forward(float input_data[10]) {\n")
            f.write("  Tensor input = NN_tensor(2, (size_t[]){ 1, 10 }, DTYPE_F32, (float *)input_data);\n")
            for layer in self.impl:
                f.write(f"  ")
                f.write(layer.getCDeclare())
                f.write("\n")
            f.write("\n")

            last_layer = "input"
            for layer in self.impl:
                f.write(f"  ")
                f.write(layer.getCForward(last_layer))
                f.write("\n")
                last_layer = layer.name + "_out"
            f.write(f"  return {last_layer};\n");
            f.write("}\n")
            f.write("\n")
            f.write("int main() {\n")
            f.write("  float *input_data = (float[]){0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};\n")
            f.write("  forward(input_data);\n")
            f.write("  return 0;\n")
            f.write("}\n")

        print("Model implementation written to model.c file.")





model = MLP()


