#!/usr/bin/env bash
set -e

# Fix ownership of output files
finish() {
    # Fix ownership of output files
    user_id=$(stat -c '%u:%g' /data)
    chown -R ${user_id} /data
}
trap finish EXIT

# Determine whether we should use fasta or fastq encoding
if [[ "$1" == "fasta" ]]; then
	export TOOL=/opt/napper/build/fasta_rle
elif [[ "$1" == "fastq" ]]; then
	export TOOL=/opt/napper/build/fastq_rle
else
	echo "napper docker invocation must specify 'fasta' or 'fastq' as first argument"
	exit 1
fi

# gather time and memory statistics, save everything to 'rle.log'
echo -e "/usr/bin/time -f '\\\\nDEBUG_MAX_MEM:%M\\\\nDEBUG_RUNTIME:%E\\\\n' $TOOL ${@:2}\n" > /data/napper.log
eval "/usr/bin/time -f '\\nDEBUG_MAX_MEM:%M\\nDEBUG_RUNTIME:%E\\n' $TOOL ${@:2}" 2>&1 | tee -a /data/napper.log

