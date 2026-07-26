#!/bin/bash

SCRIPT_NAME=$(basename "$0")
if [ "${SCRIPT_NAME}" != "template_task.sh" ]; then
  echo "я бригадир, сам не работаю"
  exit 1
fi

LOG_FILE="report_${SCRIPT_NAME}.log"
PIPE_PATH=/tmp/run
PIPE_NAME=cuckoo.pid

DATE_TIME="$(date "+%d/%m/%Y %H:%M:%S")"

echo "${DATE_TIME} [$$] Скрипт запущен" >> "${LOG_FILE}"

exec 3> "${PIPE_PATH}/${PIPE_NAME}"
echo "${SCRIPT_NAME}[$$]: how much time do I have?" >&3
exec 3>&-

DELAY=0.1
sleep "$DELAY"

exec 3< "${PIPE_PATH}/${PIPE_NAME}"
read -r N <&3
exec 3<&-

INT_N=$((N))

echo "Answer: ${INT_N}"
sleep "${INT_N}"

FINAL_DELAY=$(echo "$INT_N + $DELAY" | bc)
END_DATE_TIME="$(date "+%d/%m/%Y %H:%M:%S")"
echo "${END_DATE_TIME} [$$] Скрипт завершился, работал ${FINAL_DELAY} секунд." >> "${LOG_FILE}"

