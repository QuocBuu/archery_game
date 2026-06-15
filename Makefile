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
	$(Print) "[make sim-web-deploy] build and copy to docs/ for GitHub Pages"

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

.PHONY: sim-web-deploy
sim-web-deploy:
	$(Print) BUILD and DEPLOY to docs/
	@$(MAKE) --no-print-directory -C application/simulator web
	@mkdir -p docs
	@cp application/simulator/web/build/dist/* docs/
	$(Print) Done — commit and push docs/ to publish on GitHub Pages
