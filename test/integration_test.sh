#!/usr/bin/env bash

set -eEu -o pipefail
trap 's=$?; echo "$0: error on $0:$LINENO"; exit $s' ERR

# Executable path
WAZZA="$1"
if [[ -z "${WAZZA}" ]]; then
    echo "error: the path to the wazza excutable binary is required as first argument" >&2
    exit 1
fi
if ! [[ -x "${WAZZA}" ]]; then
    echo "error: '${WAZZA}' is not executable or does not exists" >&2
    exit 1
fi

# start the server
./"${WAZZA}" >./integration_test_server.log 2>&1 &
wazza_pid=$!

# this is a super ligth server and there is no reason why
# it should take more than a few milliseconds to start
# so one second is enough time for it to boot
sleep 1

# test the server, not the http answer itself
curl http://localhost:3004

# send interrupt signal to the server
kill -INT $wazza_pid

# wait for the server to exit
wait $wazza_pid

echo "PASS: integration test passed sucesfully"
