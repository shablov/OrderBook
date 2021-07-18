#!/bin/sh

cpupower frequency-set --governor performance
build/src/orderbook/orderbook_benchmark --benchmark_repetitions=10 --benchmark_counters_tabular=true --benchmark_report_aggregates_only=true
cpupower frequency-set --governor powersave
