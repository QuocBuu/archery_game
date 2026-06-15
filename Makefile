##############################################################################
# @brief: Repository helper targets
##############################################################################

Print = @echo "~"

.PHONY: help
help:
	$(Print) "[make sim] build and run SDL native simulator"
	$(Print) "[make sim-build] build SDL native simulator"
	$(Print) "[make sim-web] build browser simulator with Emscripten"
	$(Print) "[make sim-web-serve] build and serve browser simulator locally"

.PHONY: sim
sim:
	$(Print) RUN SDL native simulator
	@$(MAKE) --no-print-directory -C application/simulator run

.PHONY: sim-build
sim-build:
	$(Print) BUILD SDL native simulator
	@$(MAKE) --no-print-directory -C application/simulator all

.PHONY: sim-web
sim-web:
	$(Print) BUILD browser simulator
	@$(MAKE) --no-print-directory -C application/simulator web

.PHONY: sim-web-serve
sim-web-serve:
	$(Print) SERVE browser simulator at http://127.0.0.1:8000/
	@$(MAKE) --no-print-directory -C application/simulator web
	@python3 -m http.server 8000 -d application/simulator/web/build/dist
