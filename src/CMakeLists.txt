dpf_add_plugin(wolf-shaper
	TARGETS jack lv2 vst2 dssi vst3 clap
	FILES_DSP
		Utils/src/Mathf.cpp
		Structures/src/Graph.cpp 
		Structures/src/Oversampler.cpp 
		Structures/src/ParamSmooth.cpp 
		../libs/DSPFilters/source/Butterworth.cpp
		../libs/DSPFilters/source/Biquad.cpp
		../libs/DSPFilters/source/Cascade.cpp
		../libs/DSPFilters/source/Filter.cpp
		../libs/DSPFilters/source/Param.cpp
		../libs/DSPFilters/source/Butterworth.cpp
		../libs/DSPFilters/source/PoleFilter.cpp
		../libs/DSPFilters/source/RBJ.cpp
		../libs/DSPFilters/source/Bessel.cpp
		../libs/DSPFilters/source/RootFinder.cpp
		WolfShaperPlugin.cpp
	FILES_UI
		Utils/src/Mathf.cpp 
		Structures/src/Graph.cpp 
		Structures/src/Margin.cpp 
		Structures/src/Animation.cpp 
		Widgets/src/WolfWidget.cpp 
		Widgets/src/GraphNode.cpp 
		Widgets/src/GraphWidget.cpp 
		Widgets/src/NanoSwitch.cpp 
		Widgets/src/RemoveDCSwitch.cpp 
		Widgets/src/NanoButton.cpp 
		Widgets/src/ResetGraphButton.cpp 
		Widgets/src/NanoWheel.cpp 
		Widgets/src/OversampleWheel.cpp 
		Widgets/src/NanoKnob.cpp 
		Widgets/src/VolumeKnob.cpp 
		Widgets/src/ResizeHandle.cpp 
		Widgets/src/LabelBox.cpp 
		Widgets/src/LabelBoxList.cpp 
		Widgets/src/BipolarModeSwitch.cpp 
		Widgets/src/LabelContainer.cpp 
		Widgets/src/GlowingLabelsBox.cpp 
		Widgets/src/NanoLabel.cpp 
		Widgets/src/WidgetBar.cpp 
		Widgets/src/ArrowButton.cpp 
		Widgets/src/MenuWidget.cpp
		Resources/Fonts/chivo_bold.cpp 
		Resources/Fonts/chivo_italic.cpp 
		WolfShaperUI.cpp
)

set_target_properties("wolf-shaper-vst2" PROPERTIES
    OUTPUT_NAME "wolf-shaper-vst")

target_include_directories(wolf-shaper PUBLIC
	"."
	"../dpf/distrho"
	"../dpf/dgl"
	"./Structures"
	"./Widgets"
	"./Utils"
	"./Resources"
	"../libs/DSPFilters/include"
)

target_link_libraries(wolf-shaper PUBLIC Threads::Threads)
