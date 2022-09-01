import os
import argparse

ALLOWED_EXTENSIONS = ['vertex', 'fragment']

parser = argparse.ArgumentParser()

parser.add_argument('--embedded-dir', required=True)
parser.add_argument('--output-dir', required=True)

args = parser.parse_args()
allowed_files = []

for root, dirs, files in os.walk(args.embedded_dir):
    allowed_files.extend(
        [f'{root}/{f}'.replace('\\', '/')
         for f in filter(lambda x: x.split('.')[-1].lower() in ALLOWED_EXTENSIONS, files)])


def read_file(path: str) -> list[str]:
    with open(path, 'r') as f:
        return f.readlines()


strings = []
enums = []

for file in allowed_files:
    lines = list(filter(lambda x: x, [line.strip() for line in read_file(file)]))

    strings.append(lines)
    enums.append(file.split('/')[-1].upper().replace('.', '_'))

output_dir = f'{args.output_dir}/embedded/'

os.makedirs(output_dir, exist_ok=True)

with open(f'{output_dir}/resources.hpp', 'w') as header_file:
    header_file.write(
        '#pragma once\n\n'
        '#include <cstdint>\n'
        '#include <string_view>\n\n'
        'namespace orange::embedded\n'
        '{\n\n'
        'enum class ResourceID: uint32_t\n'
        '{\n'
    )

    for i, enum in enumerate(enums):
        header_file.write(f'\t{enum} = {i},\n')

    header_file.write(
        '};\n\n'
        'constexpr std::string_view resources[] = {\n'
    )

    for file_lines in strings:
        header_file.write('\t{\n')

        for line in file_lines:
            header_file.write(f'\t\t"{line}\\n"\n')

        header_file.write('\t},\n')

    header_file.write(
        '};\n\n'
        'std::string_view getResource(ResourceID id)\n'
        '{\n'
        '\treturn resources[static_cast<uint32_t>(id)];\n'
        '}\n\n'
        '}\n'
    )
