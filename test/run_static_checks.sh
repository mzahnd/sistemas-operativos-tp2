#!/bin/bash
# SPDX-License-Identifier: MIT
#
# Copyright (c) 2022 Martín E. Zahnd
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
readonly SCRIPT_DIR="$(dirname "${BASH_SOURCE[0]}")"
pushd "$SCRIPT_DIR" &> /dev/null || exit 1

readonly OUT_DIR_CPP="$SCRIPT_DIR/staticCheckOutput/cppcheck"
readonly OUT_DIR_PVS="$SCRIPT_DIR/staticCheckOutput/pvs"
readonly OUT_DIR_VALGRIND="$SCRIPT_DIR/staticCheckOutput/valgrind"

cd .. # Working in project's root directory

mkdir -p "$OUT_DIR_CPP"
mkdir -p "$OUT_DIR_PVS"
mkdir -p "$OUT_DIR_VALGRIND"

SRC_DIRS=("Kernel" "Userland")
# EXECS=()
# TEST_FILE="Makefile"


echo -e "\e[34m\e[1mCppCheck\e[0m"

for src_dir in "${SRC_DIRS[@]}"; do
    cppcheck \
        --quiet \
        --enable=all \
        --suppress=missingIncludeSystem \
        --force \
        --inconclusive \
        --language=c \
        --std=c11 \
        -I "$src_dir/include" \
        "$src_dir/" \
        > "$OUT_DIR_CPP/cpp_$src_dir.log" \
        2> "$OUT_DIR_CPP/cpp_${src_dir}_err.log"

    # Remove empty files
    if [[ ! -s "$OUT_DIR_CPP/cpp_$src_dir.log" ]]; then
        rm "$OUT_DIR_CPP/cpp_$src_dir.log"
    fi

    if [[ ! -s "$OUT_DIR_CPP/cpp_${src_dir}_err.log" ]]; then
        rm "$OUT_DIR_CPP/cpp_${src_dir}_err.log"
    fi
done

echo -en "\e[32m\e[1mDone.\e[0m"
echo " See files in '$OUT_DIR_CPP'"
echo ""
echo "-------------------------"

echo -e "\e[34m\e[1mPVS Studio\e[0m"

for src_dir in "${SRC_DIRS[@]}"; do
    make clean

    pvs-studio-analyzer trace \
        --output "$OUT_DIR_PVS/${src_dir}_strace_out" \
        -- make -j"$(nproc)"

    if [[ ! -s "$OUT_DIR_PVS/${src_dir}_strace_out" ]]; then
        echo -ne "\e[32m\e[1mError:\e[0m "
        echo -n "'pvs-studio-analyzer trace' failed: "
        echo "output file is empty. ($src_dir)"
        continue
    fi

    pvs-studio-analyzer analyze \
        --file "$OUT_DIR_PVS/${src_dir}_strace_out" \
        --threads "$(nproc)" \
        --output-file "$OUT_DIR_PVS/${src_dir}_PVS-Studio.log"

    if [[ ! -s "$OUT_DIR_PVS/${src_dir}_PVS-Studio.log" ]]; then
        echo -ne "\e[32m\e[1mError:\e[0m "
        echo -n "'pvs-studio-analyzer analyze' failed: "
        echo "output file is empty. ($src_dir)"
        continue
    fi

    plog-converter \
        --analyzer '64:1,2,3;GA:1,2,3;OP:1,2,3' \
        --renderTypes tasklist \
        --output "$OUT_DIR_PVS/${src_dir}_report.tasks" \
        "$OUT_DIR_PVS/${src_dir}_PVS-Studio.log"

    if [[ ! -s "$OUT_DIR_PVS/${src_dir}_report.tasks" ]]; then
        echo -ne "\e[32m\e[1mError:\e[0m "
        echo -n "'plog-converter' failed: "
        echo "output file is empty. ($src_dir)"
        continue
    fi

done


echo -en "\e[32m\e[1mDone.\e[0m "
echo " See report files in '$OUT_DIR_PVS'"
echo ""
echo "-------------------------"

# echo -e "\e[34m\e[1mValgrind\e[0m"
#
# for bin_file in "${EXECS[@]}"; do
#     OUTPUT_LOG="test/valgrind_out_${bin_file##*/}.log"
#     valgrind \
#         --leak-check=yes \
#         $bin_file $TEST_FILE > $OUTPUT_LOG
#
#     echo -en "\e[32m\e[1mDone file $bin_file.\e[0m"
#     echo " See '$OUTPUT_LOG'"
# done

popd &> /dev/null
