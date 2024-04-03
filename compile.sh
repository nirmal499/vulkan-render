#! /usr/bin/bash

glslc_PATH="/home/nbaskey/my_opt/glslc/install/bin"

${glslc_PATH}/glslc shaders/basic.vert -o shaders/SPIRV/basic.vert.spv
${glslc_PATH}/glslc shaders/basic.frag -o shaders/SPIRV/basic.frag.spv