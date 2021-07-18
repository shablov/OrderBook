.PHONY: fmt benchmark_orderbook test_orderbook build help

fmt: ## Format source code
	@./format.sh

benchmark_orderbook: ## Run benchmarks
	@./scripts/benchmarks.sh

test_orderbook: ## Run tests
	@./scripts/tests.sh

build: ## Build the binary file
	@./scripts/build.sh

help: ## Display this help screen
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
