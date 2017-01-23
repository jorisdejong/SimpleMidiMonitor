/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
	public MidiInputCallback,
	public Timer,
    public ComboBox::Listener,
    public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics& g) override;
    void resized() override;

	void handleIncomingMidiMessage( MidiInput* source,
		const MidiMessage& message ) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* b) override;
	virtual void timerCallback() override;

private:
    void refreshDeviceList();
	juce::StringArray messages;
    ScopedPointer<MidiInput> openInput;
    ScopedPointer<ComboBox> inputBox;
    ScopedPointer<TextButton> refreshButton;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
