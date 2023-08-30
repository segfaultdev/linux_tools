#!/bin/bash

cd mp3_autoformat
  gcc mp3_autoformat.c -o mp3_autoformat
  sudo cp mp3_autoformat /usr/local/bin/mp3_autoformat
cd ..

cd ansi_print
  gcc ansi_print.c -o ansi_print
  sudo cp ansi_print /usr/local/bin/ansi_print
cd ..
