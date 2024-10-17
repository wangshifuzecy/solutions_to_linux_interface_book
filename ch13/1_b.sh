#!/bin/bash

# Define the list of executables and files to copy
executables=("./copy_sync_1024" "./copy_sync_2048" "./copy_sync_4096")  # Add more executables as needed
files_to_copy=("file_1K" "file_10K" "file_100K" "file_1M")  # List of files to copy
destination_dir="./copy_destination"  # Destination directory for copied files
num_runs=10  # Number of times to repeat each test

# Create the destination directory if it doesn't exist
mkdir -p "$destination_dir"

# Print table header
printf "%-15s %-15s %-20s\n" "Executable" "File" "Avg Real (seconds)"
printf "%-15s %-15s %-20s\n" "----------" "-----" "---------------------"

# Loop through each executable
for exe in "${executables[@]}"; do
    for file in "${files_to_copy[@]}"; do
        total_real=0

        # Run the tests multiple times
        for ((i = 1; i <= num_runs; i++)); do
            # Get the start time in seconds with nanoseconds
            start=$(date +%s.%N)

            # Use the executable to copy the file
            $exe "$file" "$destination_dir/${file##*/}"

            # Get the end time
            end=$(date +%s.%N)

            # Calculate elapsed time
            elapsed=$(echo "$end - $start" | bc)

            # Accumulate total real time
            total_real=$(echo "$total_real + $elapsed" | bc)
        done

        # Calculate average
        avg_real=$(echo "scale=7; $total_real / $num_runs" | bc)

        # Print the results in a table row
        printf "%-15s %-15s %-20s\n" "$exe" "${file##*/}" "$avg_real"

        # Clean up copied file for next iteration
        rm -f "$destination_dir/${file##*/}"
    done
    echo
done

# Remove the destination directory
rmdir "$destination_dir"