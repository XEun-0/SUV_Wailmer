#!/bin/bash

# Find all files that start with #!/bin/bash
find . -type f -exec awk 'NR==1 && !/^#!\/bin\/bash/ {exit 1}' {} \; -print | while read -r file; do
    # Convert CRLF to LF
    sed -i 's/\r$//' "$file"
    echo "Converted line endings: $file"
done
