import yaml
from subprocess import check_output

types = "asm|__asm__|auto|bool|_Bool|char|_Complex|f16|f32|f64|u8|u16|u32|u64|s8|s16|s32|s64|h2|h3|h4|v2|v3|v4|double|float|_Imaginary|int|long|short|signed|unsigned|void|"
types += check_output("ctags -x --c-types=t ./src/* | cut -d' ' -f1 | paste -d'|' -s -", shell=True, universal_newlines=True).rstrip()
enums = check_output("ctags -x --c-types=e ./src/* | cut -d' ' -f1 | paste -d'|' -s -", shell=True, universal_newlines=True).rstrip()

path = "/Users/marcusmathiassen/Library/Application Support/Sublime Text 3/Packages/C++/C.sublime-syntax"
syntax = []

with open(path, 'r') as f:
    try:
        syntax = (yaml.safe_load(f))
    except yaml.YAMLError as exc:
        print(exc)

syntax["variables"]["windows_types"] = enums
syntax["variables"]["basic_types"] = types

with open(path, 'w') as f: f.write("%YAML 1.2\n---\n")
with open(path, 'a') as f: yaml.dump(syntax, f, default_flow_style=False, sort_keys=False)
