/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    //create a combobox to select a midi device with
    inputBox = new ComboBox();
    inputBox->setTextWhenNothingSelected("Select Midi Device...");
    inputBox->addListener(this);
    addAndMakeVisible( inputBox );
    refreshDeviceList();
    
    //button to refresh the devices
    refreshButton = new TextButton("Refresh");
    refreshButton->addListener(this);
    addAndMakeVisible( refreshButton );
    
    setSize( 600, 400 );

	startTimerHz( 30 );
}

MainContentComponent::~MainContentComponent()
{
    openInput = nullptr;
}

void MainContentComponent::paint( Graphics& g )
{
	g.fillAll( Colour( 0xff001F36 ) );

	g.setColour( Colours::whitesmoke );

	for ( int i = 0; i < messages.size(); i++ )
		g.drawText( messages[i], 10, 50 + i * 20, getWidth() - 20, 20, Justification::Flags::centredLeft, true );
}

void MainContentComponent::resized()
{
    inputBox->setBounds(10, 10, 200, 40);
    refreshButton->setBounds(220, 10, 80, 40 );
}

void MainContentComponent::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if ( comboBoxThatHasChanged == inputBox )
    {
        if ( inputBox->getSelectedId() == 0 )
        {
            openInput = nullptr;
        }
        else
        {
            MidiInput* newInput = MidiInput::openDevice( inputBox->getSelectedItemIndex(), this );
            openInput = newInput;
            if ( newInput )
                newInput->start();
            else
            {
                NativeMessageBox::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Error!", "Could not open MIDI device " + inputBox->getItemText( inputBox->getSelectedItemIndex()));
                inputBox->setSelectedId(0, sendNotification);
            }
        }
    }
}

void MainContentComponent::buttonClicked(juce::Button *b)
{
    refreshDeviceList();
}

void MainContentComponent::handleIncomingMidiMessage( MidiInput* source, const MidiMessage& message )
{
	String receivedMessage = "Device: " + source->getName() + ", Channel: " + String( message.getChannel() ) + ", ";

	if ( message.isController() )
		receivedMessage += "CC: " + String( message.getControllerNumber() ) + ", Value: " + String( message.getControllerValue() );

	else if ( message.isNoteOn() )
		receivedMessage += "Note On: " + String( message.getNoteNumber() ) + ", Value: " + String( message.getVelocity() );

	else if ( message.isNoteOff() )
		receivedMessage += "Note Off: " + String( message.getNoteNumber() ) + ", Value: " + String( message.getVelocity() );

	messages.insert ( 0, receivedMessage );
	
	int maxLength = 15;
	if ( messages.size() > maxLength )
		messages.removeRange( maxLength, messages.size() - maxLength );
	
}

void MainContentComponent::timerCallback()
{
	const MessageManagerLock lock;
	repaint();
}

void MainContentComponent::refreshDeviceList()
{
    inputBox->clear( sendNotification );
    messages.clear();
    
    //check for all connected midi devices
    juce::StringArray midiDevices = MidiInput::getDevices();
    inputBox->addItemList(midiDevices, 1);
}
