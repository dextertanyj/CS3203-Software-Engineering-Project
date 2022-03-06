#!/bin/bash

AUTOTESTER_PATH=$1;

# https://stackoverflow.com/questions/4774054/reliable-way-for-a-bash-script-to-get-the-full-path-to-itself
SCRIPT_PATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

if ! [[ -x "$AUTOTESTER_PATH" ]]; then
  echo "File $AUTOTESTER_PATH not found";
  exit 1;
fi

RESULT_PATH="$SCRIPT_PATH"/results;
TEMP_RESULT="$RESULT_PATH"/temp.txt;
COMBINED_RESULT="$RESULT_PATH"/combined.txt;
mkdir -p "$RESULT_PATH";
rm -f "$COMBINED_RESULT"

TESTNAMES=();
RESULT=();

for FILE in "$SCRIPT_PATH/"*_source.txt
do
  TESTNAME=${FILE#"$SCRIPT_PATH/"};
  TESTNAME=${TESTNAME%_*.txt};
  TEST_SOURCE="$SCRIPT_PATH"/"$TESTNAME"_source.txt;
  TEST_QUERIES="$SCRIPT_PATH"/"$TESTNAME"_queries.txt;
  TEST_OUTPUT="$RESULT_PATH"/"$TESTNAME"_out.xml;
  echo "$TESTNAME Test" >> "$COMBINED_RESULT";
  "$AUTOTESTER_PATH" "$TEST_SOURCE" "$TEST_QUERIES" "$TEST_OUTPUT" | tee "$TEMP_RESULT" | \
    grep -E "(Evaluating query|answer|exception|Missing:|Additional:)" | \
    grep -B 4 -E "(Additional:)" >> "$COMBINED_RESULT";
  RESULT[${#RESULT[@]}]=${PIPESTATUS[0]};
  if [ "${RESULT[${#RESULT[@]}-1]}" -ne 0 ]; then
    tail -n +2 "$TEMP_RESULT" >> "$COMBINED_RESULT";
  fi
  echo "" >> "$COMBINED_RESULT";
  TESTNAMES[${#TESTNAMES[@]}]="$TESTNAME";
done

rm -f "$TEMP_RESULT"

SUCCESS=0;

for IDX in "${!RESULT[@]}"
do
  if [ "${RESULT[$IDX]}" -ne 0 ]; then
    echo "ERROR: ${TESTNAMES[$IDX]} test failed exceptionally";
    SUCCESS=1;
  fi
done

if [ $SUCCESS -eq 0 ] && ! grep -E "(Evaluating)" "$COMBINED_RESULT" &> /dev/null ; then
  echo "Success";
  exit $SUCCESS;
else
  cat "$COMBINED_RESULT";
  exit "$((SUCCESS + 1))";
fi