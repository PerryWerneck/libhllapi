#!/bin/bash
make zip-debug && ln -sf $(readlink -f ipc3270c-debug-*.zip) ~/public_html/

