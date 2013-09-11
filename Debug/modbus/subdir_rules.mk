################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
modbus/mb_main.obj: ../modbus/mb_main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" --silicon_version=28 -g --define="_DEBUG" --define="LARGE_MODEL" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus/modbus" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/libs/math/FPUfastRTS/V100/include" --quiet --diag_warning=225 --issue_remarks --large_memory_model --unified_memory --float_support=fpu32 --preproc_with_compile --preproc_dependency="modbus/mb_main.pp" --obj_directory="modbus" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

modbus/mb_serial.obj: ../modbus/mb_serial.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" --silicon_version=28 -g --define="_DEBUG" --define="LARGE_MODEL" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus/modbus" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/libs/math/FPUfastRTS/V100/include" --quiet --diag_warning=225 --issue_remarks --large_memory_model --unified_memory --float_support=fpu32 --preproc_with_compile --preproc_dependency="modbus/mb_serial.pp" --obj_directory="modbus" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

modbus/mb_timer.obj: ../modbus/mb_timer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.0/bin/cl2000" --silicon_version=28 -g --define="_DEBUG" --define="LARGE_MODEL" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.0/include" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus/modbus" --include_path="C:/Users/Bruno Luiz/Documents/workspace_ccs_brunoluiz/2833xModBus" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2833x/v133/DSP2833x_common/include" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v15c/include" --include_path="C:/ti/controlSUITE/libs/math/FPUfastRTS/V100/include" --quiet --diag_warning=225 --issue_remarks --large_memory_model --unified_memory --float_support=fpu32 --preproc_with_compile --preproc_dependency="modbus/mb_timer.pp" --obj_directory="modbus" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


