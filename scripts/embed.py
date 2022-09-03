import os
import re
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('--embedded-dir', required=True)
parser.add_argument('--output-dir', required=True)
parser.add_argument('--extensions', default='')

args = parser.parse_args()
filtered_files = []

ALLOWED_EXTENSIONS = [e.lower().strip() for e in args.extensions.split(',') if e]


def is_allowed(ext: str) -> bool:
    return ext in ALLOWED_EXTENSIONS or not ALLOWED_EXTENSIONS


for root, dirs, files in os.walk(args.embedded_dir):
    filtered_files.extend(
        [f'{root}/{f}'.replace('\\', '/')
         for f in filter(lambda x: is_allowed(x.split('.')[-1].lower()), files)])


def read_file(path: str) -> bytes:
    return open(path, 'rb').read()


resources = []

for file in filtered_files:
    resources.append((re.sub(r'[.\-]+', '_', file.split('/')[-1]).upper(), read_file(file)))

include_dir = f'{args.output_dir}/include/embedded'
source_dir = f'{args.output_dir}/src'

os.makedirs(include_dir, exist_ok=True)
os.makedirs(source_dir, exist_ok=True)

with open(f'{include_dir}/resources.hpp', 'w') as header_file:
    header_file.write(
        '#pragma once\n\n'
        'namespace orange::embedded\n'
        '{\n\n'
        'enum class ResourceID : uint32_t\n'
        '{\n'
    )

    for i, (enum, _) in enumerate(resources):
        header_file.write(f'\t{enum} = {i},\n')

    header_file.write(
        '};\n\n'
        'const std::vector<unsigned char> &getResource(ResourceID id);\n\n'
        '}\n'
    )

with open(f'{source_dir}/resources.cpp', 'w') as source_file:
    source_file.write(
        '#include <cstdint>\n'
        '#include <vector>\n\n'
        '#include <embedded/resources.hpp>\n\n'
        'namespace orange::embedded\n'
        '{\n\n'
        'static const auto resources = {\n'
    )

    for _, file_bytes in resources:
        source_file.write(
            '\tstd::vector<unsigned char> {\n'
            '\t\t'
        )

        for i, byte in enumerate(file_bytes, 1):
            source_file.write(f'0x{int(byte):02x}, ')

            if i % 18 == 0:
                source_file.write('\n\t\t')

        source_file.write('},\n')

    source_file.write(
        '};\n\n'
        'const std::vector<unsigned char> &getResource(ResourceID id)\n'
        '{\n'
        '\treturn *(resources.begin() + static_cast<uint32_t>(id));\n'
        '}\n\n'
        '}\n'
    )
