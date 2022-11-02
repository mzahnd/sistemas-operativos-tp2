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
readonly SCRIPT_DIR="$(dirname ${BASH_SOURCE[0]})"
pushd "$SCRIPT_DIR" &> /dev/null

PS3="Please pick an option: "
select options in \
    "I'm in Docker: generate 'compile_commands.json'" \
    "I'm in my machine: update 'compile_commands.json'"
do
    case $options in
        "I'm in Docker: generate 'compile_commands.json'")
            pushd "../" &> /dev/null
            make clean
            bear make
            bear -a make test
            popd &> /dev/null
            echo
            echo "Done!"
            break
            ;;
        "I'm in my machine: update 'compile_commands.json'")
            pushd "../" &> /dev/null
            sudo sed -i "s@/itba/@$PWD/@" compile_commands.json
            popd &> /dev/null
            break
            ;;
        *)
            echo "Invalid option."
    esac
done

popd &> /dev/null # $SCRIPT_DIR
