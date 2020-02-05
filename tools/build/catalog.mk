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

$(REPO_ROOT)/modules/logging/build/logging.a:
	@echo "######## Building $(REPO_ROOT)/modules/logging"
	$(MAKE) -C $(REPO_ROOT)/modules/logging

$(REPO_ROOT)/modules/SegmentVideo/build/SegmentVideo.a:
	@echo "######## Building $(REPO_ROOT)/modules/SegmentVideo"
	$(MAKE) -C $(REPO_ROOT)/modules/SegmentVideo

$(REPO_ROOT)/modules/uCModules/build/uCModules.a:
	@echo "######## Building $(REPO_ROOT)/modules/uCModules"
	$(MAKE) -C $(REPO_ROOT)/modules/uCModules

clean_all:
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(REPO_ROOT)/Arduino clean
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS clean
	$(MAKE) -C $(REPO_ROOT)/modules/logging clean
	$(MAKE) -C $(REPO_ROOT)/modules/uCModules clean
	$(MAKE) -C $(REPO_ROOT)/modules/SegmentVideo clean
