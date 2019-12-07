#######################################
# Paths
#######################################
# Inputs:
# REPO_ROOT

$(REPO_ROOT)/bsp/build/bsp.a:
	@echo "######## Building $(REPO_ROOT)/bsp"
	$(MAKE) -C $(REPO_ROOT)/bsp

$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a:
	@echo "######## Building $(REPO_ROOT)/FreeRTOS"
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS

$(REPO_ROOT)/Arduino/build/Arduino.a:
	@echo "######## Building $(REPO_ROOT)/Arduino"
	$(MAKE) -C $(REPO_ROOT)/Arduino
