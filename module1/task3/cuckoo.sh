#!/bin/bash

PIPE_PATH=/tmp/run
PIPE_NAME=cuckoo.pid
LOG_FILE="cuckoo.log"

mkdir -p "$PIPE_PATH"
if [[ ! -p "${PIPE_PATH}/${PIPE_NAME}" ]]; then
    mkfifo "${PIPE_PATH}/${PIPE_NAME}"
fi

echo "$(date "+%d/%m/%Y %H:%M:%S") Startup!" >> "$LOG_FILE"

exec 3<> "${PIPE_PATH}/${PIPE_NAME}"

kill() {
    echo "$(date "+%d/%m/%Y %H:%M:%S") Shutdown!" >> "$LOG_FILE"
    exec 3>&-
    exit 0
}
trap kill SIGTERM SIGINT SIGQUIT SIGKILL

while true; do
    if read -r message <&3; then
        if [[ "$message" =~ ^([A-Za-z0-9_]+)\[([0-9]+)\]:\ how\ much\ time\ do\ I\ have\?$ ]]; then
            NAME="${BASH_REMATCH[1]}"
            PID="${BASH_REMATCH[2]}"
            N=$((RANDOM % 9 + 2))

            echo "$NAME[$PID] $N"
            echo "$(date "+%d/%m/%Y %H:%M:%S") $NAME[$PID] $N" >> "$LOG_FILE"
        else
            echo "$(date "+%d/%m/%Y %H:%M:%S") ERROR: malformed request: $message" >> "$LOG_FILE"
        fi
    fi
done
