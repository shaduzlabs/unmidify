/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#include <catch.hpp>
#include <unmidify.hpp>

using namespace std::placeholders;

namespace sl
{
namespace midi
{
namespace test
{

class TestMidiListenerCallbacks : public Unmidify
{
public:
  void onNoteOff(NoteOff msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::NoteOff);
    MidiNote note(MidiNote::Name::CSharp, 8);
    CHECK(note == msg_.getNote());
    CHECK(0x41 == msg_.getVelocity());
    CHECK(3 == msg_.data().size());
  }

  void onNoteOn(NoteOn msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::NoteOn);
    MidiNote note(MidiNote::Name::D, 8);
    CHECK(note == msg_.getNote());
    CHECK(0x42 == msg_.getVelocity());
    CHECK(3 == msg_.data().size());
  }

  void onPolyPressure(PolyPressure msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::PolyPressure);
    MidiNote note(0x63);
    CHECK(note == msg_.getNote());
    CHECK(0x43 == msg_.getPressure());
    CHECK(3 == msg_.data().size());
  }

  void onControlChange(ControlChange msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::ControlChange);
    CHECK(0x64 == msg_.getControl());
    CHECK(0x44 == msg_.getValue());
    CHECK(3 == msg_.data().size());
  }

  void onProgramChange(ProgramChange msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::ProgramChange);
    CHECK(0x65 == msg_.getProgram());
    CHECK(2 == msg_.data().size());
  }

  void onChannelPressure(ChannelPressure msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::ChannelPressure);
    CHECK(0x66 == msg_.getPressure());
    CHECK(2 == msg_.data().size());
  }

  void onPitchBend(PitchBend msg_) override
  {
    CHECK(msg_.getType() == MidiMessage::Type::PitchBend);
    CHECK(0x8FF == msg_.getPitch());
    CHECK(3 == msg_.data().size());
  }
};

//--------------------------------------------------------------------------------------------------

TEST_CASE("MidiNote comparison", "MidiNote")
{
  MidiNote noteC0_a{MidiNote::Name::C, 0};
  MidiNote noteC0_b{0};

  MidiNote noteA7_a{MidiNote::Name::A, 7};
  MidiNote noteA7_b{93};

  CHECK(noteC0_a == noteC0_b);
  CHECK(noteC0_a != noteA7_a);
  CHECK(noteA7_a == noteA7_b);
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("MidiNote getters and setters", "MidiNote")
{
  MidiNote note{MidiNote::Name::A, 7};
  CHECK(MidiNote::Name::A == note.getNote());
  CHECK(93 == note.getNoteValue());
  CHECK(7 == note.getOctave());

  note.setNote(MidiNote::Name::BFlat);
  CHECK(MidiNote::Name::BFlat == note.getNote());
  CHECK(94 == note.getNoteValue());
  CHECK(7 == note.getOctave());

  note.setNoteValue(13);
  CHECK(MidiNote::Name::CSharp == note.getNote());
  CHECK(13 == note.getNoteValue());
  CHECK(1 == note.getOctave());

  note.setOctave(8);
  CHECK(MidiNote::Name::CSharp == note.getNote());
  CHECK(97 == note.getNoteValue());
  CHECK(8 == note.getOctave());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("NoteOff message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch9};
  NoteOff message(channel, 0x71, 0x7F);
  MidiNote note = message.getNote();
  uint8_t velocity = message.getVelocity();
  CHECK(0x71 == note.getNoteValue());
  CHECK(0x7F == velocity);
  CHECK(MidiMessage::Channel::Ch9 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("NoteOn message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch9};
  NoteOn message(channel, 0x1E, 0xFF);
  MidiNote note = message.getNote();
  uint8_t velocity = message.getVelocity();
  CHECK(0x1E == note.getNoteValue());
  CHECK(0x7F == velocity);
  CHECK(MidiMessage::Channel::Ch9 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("PolyPressure message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch15};
  PolyPressure message(channel, 0x43, 0x1B);
  MidiNote note = message.getNote();
  uint8_t pressure = message.getPressure();
  CHECK(0x43 == note.getNoteValue());
  CHECK(0x1B == pressure);
  CHECK(MidiMessage::Channel::Ch15 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ControlChange message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch3};
  ControlChange message(channel, 0x11, 0x67);
  uint8_t value = message.getValue();
  CHECK(0x11 == message.getControl());
  CHECK(0x67 == value);
  CHECK(MidiMessage::Channel::Ch3 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ProgramChange message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch2};
  ProgramChange message(channel, 0x03);
  CHECK(0x03 == message.getProgram());
  CHECK(MidiMessage::Channel::Ch2 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("ChannelPressure message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch14};
  ChannelPressure message(channel, 0x44);
  CHECK(0x44 == message.getPressure());
  CHECK(MidiMessage::Channel::Ch14 == message.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("PitchBend message", "MidiMessage")
{
  MidiMessage::Channel channel{MidiMessage::Channel::Ch7};
  PitchBend message1(channel, 0x01, 0x23);
  PitchBend message2(channel, 0xff, 0xff);
  PitchBend message3(channel, 0x0123);
  PitchBend message4(channel, 0xffff);

  CHECK(0x1181 == message1.getPitch());
  CHECK(0x3FFF == message2.getPitch());
  CHECK(0x0123 == message3.getPitch());
  CHECK(0x3FFF == message4.getPitch());

  CHECK(MidiMessage::Channel::Ch7 == message1.getChannel());
  CHECK(MidiMessage::Channel::Ch7 == message2.getChannel());
  CHECK(MidiMessage::Channel::Ch7 == message3.getChannel());
  CHECK(MidiMessage::Channel::Ch7 == message4.getChannel());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Clock messages", "MidiMessage")
{
  Clock message1{MidiMessage::Type::TimingClock};
  Clock message2{MidiMessage::Type::Start};
  Clock message3{MidiMessage::Type::Continue};
  Clock message4{MidiMessage::Type::Stop};
  Clock message5{MidiMessage::Type::NoteOn};

  CHECK(Clock::Type::ClockTick == message1.getClockType());
  CHECK(Clock::Type::Start == message2.getClockType());
  CHECK(Clock::Type::Continue == message3.getClockType());
  CHECK(Clock::Type::Stop == message4.getClockType());
  CHECK(Clock::Type::Unknown == message5.getClockType());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("SysEx messages", "MidiMessage")
{
  // 0x00 0x21 0x02 - Mutable Instruments
  SysEx msgSysEx_1(
    {0xF0, 0x00, 0x21, 0x02, 0x00, 0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xf7});
  SysEx msgSysEx_2(
    {0xF0, 0x00, 0x21, 0x02, 0x00, 0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09});

  CHECK(3 == msgSysEx_1.getHeader().size());
  CHECK(11 == msgSysEx_1.getPayload().size());

  CHECK(3 == msgSysEx_2.getHeader().size());
  CHECK(11 == msgSysEx_2.getPayload().size());

  CHECK(msgSysEx_1.getPayload().size() == msgSysEx_2.getPayload().size());

  // 0x41 - Roland Corporation
  SysEx msgSysEx_3(
    {0xF0, 0x41, 0x00, 0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xf7});
  CHECK(1 == msgSysEx_3.getHeader().size());
  CHECK(11 == msgSysEx_3.getPayload().size());
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Universal SysEx Realtime messages", "MidiMessage")
{
  //! \todo implement
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("Universal SysEx Non-Realtime messages", "MidiMessage")
{
  //! \todo implement
}

//--------------------------------------------------------------------------------------------------

TEST_CASE("MidiMessageListener callbacks", "Unmidify")
{

  TestMidiListenerCallbacks midiListener;

  NoteOff msgNoteOff(MidiMessage::Channel::Ch1, 0x61, 0x41);
  midiListener.process(msgNoteOff.data());

  NoteOn msgNoteOn(MidiMessage::Channel::Ch2, 0x62, 0x42);
  midiListener.process(msgNoteOn.data());

  PolyPressure msgPolyPressure(MidiMessage::Channel::Ch3, 0x63, 0x43);
  midiListener.process(msgPolyPressure.data());

  ControlChange msgControlChange(MidiMessage::Channel::Ch4, 0x64, 0x44);
  midiListener.process(msgControlChange.data());

  ProgramChange msgProgramChange(MidiMessage::Channel::Ch5, 0x65);
  midiListener.process(msgProgramChange.data());

  ChannelPressure msgChannelPressure(MidiMessage::Channel::Ch6, 0x66);
  midiListener.process(msgChannelPressure.data());

  PitchBend msgPitchBend(MidiMessage::Channel::Ch7, 0x7F, 0x11);
  midiListener.process(msgPitchBend.data());
}

//--------------------------------------------------------------------------------------------------

} // test
} // midi
} // sl
