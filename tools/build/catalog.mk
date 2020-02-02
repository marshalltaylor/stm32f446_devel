#######################################
# Paths
#######################################
# Inputs:
# REPO_ROOT
# board

$(REPO_ROOT)/bsp/build/bsp_$(board).a:
	@echo "######## Building $(REPO_ROOT)/bsp_$(board)"
	$(MAKE) -C $(REPO_ROOT)/bsp board=$(board)

$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a:
	@echo "######## Building $(REPO_ROOT)/FreeRTOS"
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS

$(REPO_ROOT)/Arduino/build/Arduino.a:
	@echo "######## Building $(REPO_ROOT)/Arduino"
	$(MAKE) -C $(REPO_ROOT)/Arduino

$(REPO_ROOT)/drivers/logging/build/logging.a:
	@echo "######## Building $(REPO_ROOT)/drivers/logging"
	$(MAKE) -C $(REPO_ROOT)/drivers/logging

$(REPO_ROOT)/drivers/SegmentVideo/build/SegmentVideo.a:
	@echo "######## Building $(REPO_ROOT)/drivers/SegmentVideo"
	$(MAKE) -C $(REPO_ROOT)/drivers/SegmentVideo

$(REPO_ROOT)/drivers/uCModules/build/uCModules.a:
	@echo "######## Building $(REPO_ROOT)/drivers/uCModules"
	$(MAKE) -C $(REPO_ROOT)/drivers/uCModules
