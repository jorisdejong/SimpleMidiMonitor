/*
  ==============================================================================

  This file was auto-generated!

  ==============================================================================
  */

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
	setSize( 600, 400 );

	//check for all connected midi devices
	juce::StringArray midiDevices = MidiInput::getDevices();

	//try to open them all

	for ( int i = 0; i < midiDevices.size(); i++ )
	{
		MidiInput* newInput = MidiInput::openDevice( i, this );

		if ( newInput )
		{
			newInput->start();
			openInputs.add( newInput );
		}
		else
			delete newInput;
	}

	startTimerHz( 30 );
}

MainContentComponent::~MainContentComponent()
{
	openInputs.clear();
}

void MainContentComponent::paint( Graphics& g )
{
	g.fillAll( Colour( 0xff001F36 ) );

	g.setColour( Colours::whitesmoke );

	for ( int i = 0; i < messages.size(); i++ )
		g.drawText( messages[i], 10, i * 20, getWidth() - 20, 20, Justification::Flags::centredLeft, true );
}

void MainContentComponent::resized()
{
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
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
