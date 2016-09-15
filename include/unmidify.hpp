/*
        ##########    Copyright (C) 2015 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #####*/

#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <vector>

namespace sl
{

namespace midi
{

using tRawData = std::vector<uint8_t>;

//--------------------------------------------------------------------------------------------------

#define M_MIDI_BYTE(data) static_cast<uint8_t>(0x7F & data)
#define M_UINT8(data) static_cast<uint8_t>(data)

//--------------------------------------------------------------------------------------------------

/**
* @defgroup Midi MIDI
* @{
*/

/**
* @defgroup Basics Basics
* @ingroup Midi
* @{
*/

/**
\class MidiNote
\brief Class representing a single midi note
*/

class MidiNote
{
public:
  //! The notes
  enum class Name : uint8_t
  {
    C,
    CSharp,
    DFlat = CSharp,
    D,
    DSharp,
    EFlat = DSharp,
    E,
    F,
    FSharp,
    GFlat = FSharp,
    G,
    GSharp,
    AFlat = GSharp,
    A,
    ASharp,
    BFlat = ASharp,
    B,
  };

  //! Constructor
  /*!
			\param note_  The Midi note
			\param octave_  The octave
			*/
  MidiNote(Name note_, uint8_t octave_)
    : m_note(note_), m_octave(std::min<uint8_t>(10, std::max<uint8_t>(0, octave_)))
  {
  }

  //! Constructor
  //! \param note_  The Midi note number in the range [0-127]
  MidiNote(uint8_t note_)
  {
    setNoteValue(note_);
  }

  //! Comparison (equality)
  //! \return true if the two MidiNote objects are equal
  bool operator==(const MidiNote& other_) const
  {
    return (m_note == other_.m_note) && (m_octave == other_.m_octave);
  }

  //! Comparison (inequality)
  //! \return true if the two MidiNote objects are not equal
  bool operator!=(const MidiNote& other_) const
  {
    return !(operator==(other_));
  }

  //! Set the Midi note
  //! \param note_ The Midi note
  void setNote(Name note_)
  {
    m_note = note_;
  }

  //! Set the Midi note
  //! \param note_ The Midi note number in the range [0-127]
  void setNoteValue(uint8_t note_)
  {
    m_octave = static_cast<uint8_t>(M_MIDI_BYTE(note_) / 12U);
    m_note = static_cast<Name>(note_ % 12U);
  }

  //! Get the note
  //! \return The note
  Name getNote()
  {
    return m_note;
  }

  //! Get the value of the midi note
  //! \return The value of the note in the range [0-127]
  uint8_t getNoteValue()
  {
    if (m_octave >= 10U)
    {
      return M_UINT8(m_octave * 12) + std::min<uint8_t>(7, M_UINT8(m_note));
    }
    return M_UINT8((m_octave * 12) + M_UINT8(m_note));
  }

  //! Set the octave
  //! \param octave_ The octave in the range [0-10]
  void setOctave(uint8_t octave_)
  {
    m_octave = octave_ > 10U ? 10U : octave_;
  }

  //! Get the octave of the midi note
  //! \return The value of the octave in the range [0-10]
  uint8_t getOctave()
  {
    return m_octave;
  }

private:
  Name m_note;
  uint8_t m_octave;
};

/**
\class MidiMessage
\brief

Full MIDI messages specification:
http://www.midi.org/techspecs/midimessages.php
*/
class MidiMessage
{
public:
  //! All the known Midi message types
  enum class Type : uint8_t
  {
    NoteOff = 0x80,         //!<  Sent when a note is released (ended)
    NoteOn = 0x90,          //!<  Sent when a note is depressed (start)
    PolyPressure = 0xA0,    //!<  Most often sent by pressing down on the key after it "bottoms out"
    ControlChange = 0xB0,   //!<  Sent when a controller value changes
    ProgramChange = 0xC0,   //!<  Sent when the patch number changes
    ChannelPressure = 0xD0, //!<  The highest pressure value of all the current depressed keys
    PitchBend = 0xE0,       //!<  Indicates a change in the pitch bender
    SysEx = 0xF0,           //!<  System Exclusive
    MTC = 0xF1,             //!<  MIDI Time Code Quarter Frame
    SongPosition = 0xF2,    //!<  Song Position Pointer
    SongSelect = 0xF3,      //!<  Song Select
    Reserved_0 = 0xF4,      //!<  Undefined(Reserved)
    Reserved_1 = 0xF5,      //!<  Undefined(Reserved)
    TuneRequest = 0xF6,     //!<  Tune Request
    SysExEnd = 0xF7,        //!<  Terminates a System Exclusive dump
    TimingClock = 0xF8,     //!<  Timing Clock
    Reserved_2 = 0xF9,      //!<  Undefined(Reserved)
    Start = 0xFA,           //!<  Start the current sequence playing
    Continue = 0xFB,        //!<  Continue at the point the sequence was Stopped
    Stop = 0xFC,            //!<  Stop the current sequence
    Reserved_3 = 0xFD,      //!<  Undefined(Reserved)
    ActiveSensing = 0xFE,   //!<  Active Sensing
    Reset = 0xFF,           //!<  Reset all receivers in the system to power-up status
  };

  //! The Midi channels
  enum class Channel : uint8_t
  {
    Ch1,       //!<  Channel 1
    Ch2,       //!<  Channel 2
    Ch3,       //!<  Channel 3
    Ch4,       //!<  Channel 4
    Ch5,       //!<  Channel 5
    Ch6,       //!<  Channel 6
    Ch7,       //!<  Channel 7
    Ch8,       //!<  Channel 8
    Ch9,       //!<  Channel 9
    Ch10,      //!<  Channel 10
    Ch11,      //!<  Channel 11
    Ch12,      //!<  Channel 12
    Ch13,      //!<  Channel 13
    Ch14,      //!<  Channel 14
    Ch15,      //!<  Channel 15
    Ch16,      //!<  Channel 16
    Undefined, //!<  Undefined/Unknown
  };

  //! Constructor
  /*!
			\param type_  The MidiMessage type
			*/
  MidiMessage(Type type_) : m_type(type_)
  {
  }

  MidiMessage(const MidiMessage&) = delete;

  MidiMessage& operator=(const MidiMessage&) = delete;

  virtual const tRawData& data() const = 0;
  Type getType() const
  {
    return m_type;
  }

protected:
  virtual tRawData& _data() = 0;

private:
  friend class Unmidify;
  Type m_type;
};

//--------------------------------------------------------------------------------------------------

/**
\class MidiMessage
\brief Class representing a MIDI message

*/

template <midi::MidiMessage::Type MsgType>
class MidiMessageBase : public midi::MidiMessage
{
public:
  //! Constructor
  /*!
			\param data_ The raw message data
	*/
  MidiMessageBase(tRawData data_) : midi::MidiMessage(MsgType), m_data(std::move(data_))
  {
  }

  //! Constructor
  /*!
			\param header_ The raw message header
			\param data_   The raw message data
			*/
  MidiMessageBase(const tRawData& header_, const tRawData& data_)
    : midi::MidiMessage(MsgType), m_data(header_)
  {
    m_data.insert(m_data.end(), data_.begin(), data_.end());
  }

  //! Constructor
  /*!
			\param channel_  The MIDI channel
			\param data_     The raw message data
			*/
  MidiMessageBase(MidiMessage::Channel channel_, const tRawData& data_)
    : midi::MidiMessage(MsgType), m_data{M_UINT8((M_UINT8(channel_) | M_UINT8(getType())))}
  {
    m_data.insert(m_data.end(), data_.begin(), data_.end());
  }

  virtual ~MidiMessageBase()
  {
  }

  const tRawData& data() const override
  {
    return m_data;
  }

  MidiMessage::Channel getChannel() const
  {
    return (m_data.size() == 0 || getType() < MidiMessage::Type::SysEx)
             ? static_cast<MidiMessage::Channel>(m_data[0] & 0x0F)
             : MidiMessage::Channel::Undefined;
  }

  bool operator==(const MidiMessageBase& other_) const
  {
    return (m_data.size() == other_.m_data.size())
           && (std::equal(m_data.begin(), m_data.end(), other_.m_data.begin()));
  }

  bool operator!=(const MidiMessageBase& other_) const
  {
    return !(operator==(other_));
  }

protected:
  virtual tRawData& _data() override
  {
    return m_data;
  }

private:
  tRawData m_data;
};

/** @} */ // End of group Basics

//--------------------------------------------------------------------------------------------------

/**
* @defgroup ChannelMessages Channel messages
* @ingroup Midi
* @{
*/

/**
\class NoteOff
\brief A NoteOff MIDI message
*/

class NoteOff : public midi::MidiMessageBase<midi::MidiMessage::Type::NoteOff>
{
public:
  //! Constructor from channel, note and velocity
  /*!
			\param channel_   The channel
			\param note_      The midi note in the range [0-127]
			\param velocity_  The velocity in the range [0-127]
			*/
  NoteOff(MidiMessage::Channel channel_, uint8_t note_, uint8_t velocity_ = 0)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(note_), M_MIDI_BYTE(velocity_)})
  {
  }

  MidiNote getNote() const
  {
    return MidiNote(data()[1]);
  }

  uint8_t getVelocity() const
  {
    return data()[2];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class NoteOn
\brief A NoteOn MIDI message
*/

class NoteOn : public midi::MidiMessageBase<midi::MidiMessage::Type::NoteOn>
{
public:
  //! Constructor from channel, note and velocity
  /*!
			\param channel_   The channel
			\param note_      The midi note in the range [0-127]
			\param velocity_  The velocity in the range [0-127]
			*/
  NoteOn(MidiMessage::Channel channel_, uint8_t note_, uint8_t velocity_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(note_), M_MIDI_BYTE(velocity_)})
  {
  }

  MidiNote getNote() const
  {
    return MidiNote(data()[1]);
  }

  uint8_t getVelocity() const
  {
    return data()[2];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class PolyPressure
\brief A PolyPressure MIDI message
*/

class PolyPressure : public midi::MidiMessageBase<midi::MidiMessage::Type::PolyPressure>
{
public:
  //! Constructor from channel, note and pressure value
  /*!
			\param channel_   The channel
			\param note_      The midi note in the range [0-127]
			\param pressure_  The pressure value in the range [0-127]
			*/
  PolyPressure(MidiMessage::Channel channel_, uint8_t note_, uint8_t pressure_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(note_), M_MIDI_BYTE(pressure_)})
  {
  }

  MidiNote getNote() const
  {
    return MidiNote(data()[1]);
  }

  uint8_t getPressure() const
  {
    return data()[2];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class ControlChange
\brief A ControlChange MIDI message
*/

class ControlChange : public midi::MidiMessageBase<midi::MidiMessage::Type::ControlChange>
{
public:
  //! Constructor from channel, controller number and controller value
  /*!
			\param channel_  The channel
			\param control_  The controller number in the range [0-127]
			\param value_    The controller value in the range [0-127]
			*/
  ControlChange(MidiMessage::Channel channel_, uint8_t control_, uint8_t value_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(control_), M_MIDI_BYTE(value_)})
  {
  }

  uint8_t getControl() const
  {
    return data()[1];
  }

  uint8_t getValue() const
  {
    return data()[2];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class ProgramChange
\brief A ProgramChange MIDI message
*/

class ProgramChange : public midi::MidiMessageBase<midi::MidiMessage::Type::ProgramChange>
{
public:
  ProgramChange(MidiMessage::Channel channel_, uint8_t program_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(program_)})
  {
  }

  uint8_t getProgram() const
  {
    return data()[1];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class ChannelPressure
\brief A ChannelPressure MIDI message
*/

class ChannelPressure : public midi::MidiMessageBase<midi::MidiMessage::Type::ChannelPressure>
{
public:
  ChannelPressure(MidiMessage::Channel channel_, uint8_t pressure_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(pressure_)})
  {
  }

  uint8_t getPressure() const
  {
    return data()[1];
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class PitchBend
\brief A PitchBend MIDI message
*/

class PitchBend : public midi::MidiMessageBase<midi::MidiMessage::Type::PitchBend>
{
public:
  PitchBend(MidiMessage::Channel channel_, uint8_t pitchL_, uint8_t pitchH_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(pitchL_), M_MIDI_BYTE(pitchH_)})
  {
  }

  PitchBend(MidiMessage::Channel channel_, uint16_t pitch_)
    : MidiMessageBase(channel_, {M_MIDI_BYTE(pitch_), M_MIDI_BYTE(pitch_ >> 7)})
  {
  }

  uint16_t getPitch() const
  {
    return static_cast<uint16_t>(data()[1] | (data()[2] << 7));
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class Clock
\brief A MIDI beat clock message
*/

class Clock : public midi::MidiMessageBase<midi::MidiMessage::Type::TimingClock>
{
public:
  enum class Type : uint8_t
  {
    Unknown,   //!< Unknown
    ClockTick, //!< Clock tick
    Start,     //!< Start
    Continue,  //!< Continue
    Stop,      //!< Stop
  };

  Clock(MidiMessage::Type type_) : MidiMessageBase(tRawData())
  {
    switch (type_)
    {
      case MidiMessage::Type::TimingClock:
      {
        m_type = Type::ClockTick;
        break;
      }
      case MidiMessage::Type::Start:
      {
        m_type = Type::Start;
        break;
      }
      case MidiMessage::Type::Continue:
      {
        m_type = Type::Continue;
        break;
      }
      case MidiMessage::Type::Stop:
      {
        m_type = Type::Stop;
        break;
      }
      default:
        break;
    }
  }

  Type getClockType() const
  {
    return m_type;
  }

private:
  Type m_type{Type::Unknown};
};

/** @} */ // End of group ChannelMessages

//--------------------------------------------------------------------------------------------------

/**
* @defgroup SystemMessages System messages
* @ingroup Midi
* @{
*/

/**
\class SysEx
\brief A SysEx MIDI message
*/

class SysEx : public midi::MidiMessageBase<midi::MidiMessage::Type::SysEx>
{
public:
  //! Constructor from raw data
  //! \param data_  The raw data
  SysEx(tRawData data_) : MidiMessageBase(std::move(data_))
  {
    checkAndSetStartAndStopBytes();
  }

  SysEx(const tRawData& header_, const tRawData& data_) : MidiMessageBase(header_, data_)
  {
    checkAndSetStartAndStopBytes();
  }

  SysEx(uint8_t manufacturerId, const tRawData& data_) : SysEx({0xF0, manufacturerId}, data_)
  {
  }

  SysEx(uint8_t manufacturerIdHi, uint8_t manufacturerIdLo, const tRawData& data_)
    : SysEx({0xF0, 0, manufacturerIdHi, manufacturerIdLo}, data_)
  {
  }

  tRawData getHeader() const
  {
    return tRawData(data().begin() + 1, data().begin() + 1 + getHeaderLength());
  }

  tRawData getPayload() const
  {
    size_t headerLength = getManufacturerIdLength() + 1;
    size_t payloadLength = data().size() - headerLength - ((data().back() == 0xF7) ? 1 : 0);

    return tRawData(data().begin() + headerLength, data().begin() + headerLength + payloadLength);
  }

  virtual size_t getHeaderLength() const
  {
    return getManufacturerIdLength();
  }

private:
  size_t getManufacturerIdLength() const
  {
    return (data()[1] == 0) ? 3 : 1;
  }

  void checkAndSetStartAndStopBytes()
  {
    if (_data().front() != 0xF0)
    {
      _data().insert(data().begin(), 0xF0);
    }
    if (_data().back() != 0xF7)
    {
      _data().push_back(0xF7);
    }
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class USysExNonRT
\brief An Universal SysEx Non-RealTime MIDI message
*/

class USysExNonRT : public SysEx
{
public:
  enum class MessageId
  {
    SampleDumpHeader = 0x01,     //!< Sample dump header
    SampleDumpData = 0x02,       //!< Sample Data Packet
    SampleDumpRequest = 0x03,    //!< Sample Dump Request
    MIDITimeCode = 0x04,         //!< MIDI Time Code
    SampleDumpExtensions = 0x05, //!< Sample Dump Extensions
    GeneralInformation = 0x06,   //!< General Information
    FileDump = 0x07,             //!< File Dump
    MIDITuning = 0x08,           //!< MIDI Tuning Standard (Non-Real Time)
    GeneralMIDI = 0x09,          //!< General MIDI
    DownloadableSounds = 0x0A,   //!< Downloadable Sounds
    FileReferenceMessage = 0x0B, //!< File Reference Message
    MIDIVisualControl = 0x0C,    //!< MIDI Visual Control

    GenericHandshakingEOF = 0x7B,    //!< Generic handshaking - End of File
    GenericHandshakingWait = 0x7C,   //!< Generic handshaking - Wait
    GenericHandshakingCancel = 0x7D, //!< Generic handshaking - Cancel
    GenericHandshakingNAK = 0x7E,    //!< Generic handshaking - NAK
    GenericHandshakingACK = 0x7F,    //!< Generic handshaking - ACK

    Unknown, //!< Unknown
  };

  USysExNonRT(tRawData data_) : SysEx(std::move(data_))
  {
  }

  USysExNonRT(const tRawData& header_, const tRawData& data_) : SysEx(header_, data_)
  {
  }

  USysExNonRT(MessageId msgId_, const tRawData& data_)
    : SysEx({0xF0, 0x7E, 0x00, M_UINT8(msgId_)}, data_)
  {
  }

  uint8_t getTargetDevice() const
  {
    return data()[2];
  }

  MessageId getMessageId() const
  {
    uint8_t m = data()[3];
    if ((m > M_UINT8(MessageId::MIDIVisualControl) && m < M_UINT8(MessageId::GenericHandshakingEOF))
        || m > M_UINT8(MessageId::GenericHandshakingACK))
    {
      return MessageId::Unknown;
    }
    return static_cast<MessageId>(m);
  }

  uint8_t getSubId2() const
  {
    return getHeaderLength() == 4 ? data()[4] : 0;
  }

private:
  size_t getHeaderLength() const override
  {
    uint8_t m = data()[3];
    if (m <= M_UINT8(MessageId::SampleDumpRequest)
        || (m >= M_UINT8(MessageId::GenericHandshakingEOF)
             && m <= M_UINT8(MessageId::GenericHandshakingACK)))
    {
      return 3;
    }
    return 4;
  }
};

//--------------------------------------------------------------------------------------------------

/**
\class USysExRT
\brief An Universal SysEx RealTime MIDI message
*/

class USysExRT : public SysEx
{
public:
  enum class MessageId : uint8_t
  {
    MIDITimeCode = 0x01,                 //!< MIDI Time Code
    MIDIShowControl = 0x02,              //!< MIDI Show Control
    NotationInformation = 0x03,          //!< Notation Information
    DeviceControl = 0x04,                //!< Device Control
    RealTimeMTCCueing = 0x05,            //!< Real Time MTC Cueing
    MIDIMachineControlCommands = 0x06,   //!< MIDI Machine Control Commands
    MIDIMachineControlResponses = 0x07,  //!< MIDI Machine Control Responses
    MIDITuning = 0x08,                   //!< MIDI Tuning Standard (Real Time)
    ControllerDestinationSetting = 0x09, //!< Controller Destination Setting
    KeyBasedInstrumentControl = 0x0A,    //!< Key-based Instrument Control
    ScalablePoliphonyMIP = 0x0B,         //!< Scalable Polyphony MIDI MIP Message
    MobilePhoneControlMessage = 0x0C,    //!< Mobile Phone Control Message
    Unknown                              //!< Unkown
  };

  USysExRT(tRawData data_) : SysEx(std::move(data_))
  {
  }

  USysExRT(const tRawData& header_, const tRawData& data_) : SysEx(header_, data_)
  {
  }

  USysExRT(MessageId msgId_, const tRawData& data_)
    : SysEx({0xF0, 0x7F, 0x00, M_UINT8(msgId_)}, data_)
  {
  }

  uint8_t getTargetDevice() const
  {
    return data()[2];
  }

  MessageId getMessageId() const
  {
    uint8_t m = data()[3];
    if (m > M_UINT8(MessageId::MobilePhoneControlMessage))
    {
      return MessageId::Unknown;
    }
    return static_cast<MessageId>(m);
  }

  uint8_t getSubId2() const
  {
    return data()[4];
  }

private:
  size_t getHeaderLength() const override
  {
    return 4;
  }
};

/** @} */ // End of group SystemMessages

//--------------------------------------------------------------------------------------------------

/**
* @defgroup Utilities Utilities
* @ingroup Midi
* @{
*/

/**
\class Unmidify
\brief A general purpose MIDI message listener class
*/

class Unmidify
{
public:
  static void process(double timeStamp_, std::vector<unsigned char>* pMessage_, void* pUserData_)
  {
    if (nullptr == pMessage_ || nullptr == pUserData_)
    {
      return;
    }

    Unmidify* pSelf = static_cast<Unmidify*>(pUserData_);
    pSelf->process({pMessage_->begin(), pMessage_->end()});
  }

  virtual ~Unmidify()
  {
  }

  void process(const tRawData& data_)
  {
    std::unique_ptr<MidiMessage> message = parseMidiMessage(data_);
    if (message)
    {

#define M_MESSAGE_CB3(idMsg)                                                                      \
  case MidiMessage::Type::idMsg:                                                                  \
  {                                                                                               \
    if (message->_data().size() != 3)                                                             \
    {                                                                                             \
      break;                                                                                      \
    }                                                                                             \
    MidiMessage::Channel channel = static_cast<MidiMessage::Channel>(message->_data()[0] & 0x0F); \
    on##idMsg({channel, message->_data()[1], message->_data()[2]});                               \
    break;                                                                                        \
  }
#define M_MESSAGE_CB2(idMsg)                                                                      \
  case MidiMessage::Type::idMsg:                                                                  \
  {                                                                                               \
    if (message->_data().size() != 2)                                                             \
    {                                                                                             \
      break;                                                                                      \
    }                                                                                             \
    MidiMessage::Channel channel = static_cast<MidiMessage::Channel>(message->_data()[0] & 0x0F); \
    on##idMsg({channel, message->_data()[1]});                                                    \
    break;                                                                                        \
  }

      switch (message->getType())
      {
        M_MESSAGE_CB3(NoteOff);
        M_MESSAGE_CB3(NoteOn);
        M_MESSAGE_CB3(PolyPressure);
        M_MESSAGE_CB3(ControlChange);
        M_MESSAGE_CB2(ProgramChange);
        M_MESSAGE_CB2(ChannelPressure);
        M_MESSAGE_CB3(PitchBend);
        case MidiMessage::Type::TimingClock:
        case MidiMessage::Type::Start:
        case MidiMessage::Type::Continue:
        case MidiMessage::Type::Stop:
        {
          onClock({message->getType()});
          break;
        }
        case MidiMessage::Type::SysEx:
        {
          if (message->data()[1] == 0x7E)
          {
            onUSysExNonRT({std::move(message->_data())});
          }
          else if (message->data()[1] == 0x7F)
          {
            onUSysExRT({std::move(message->_data())});
          }
          else
          {
            onSysEx({std::move(message->_data())});
          }
          break;
        }
        default:
          break;
      }
#undef M_MESSAGE_CB3
#undef M_MESSAGE_CB2
    }
  }

  //! The callback function invoked when a NoteOff message is received
  //! \param msg_   The NoteOff message
  virtual void onNoteOff(NoteOff msg_)
  {
  }

  //! The callback function invoked when a NoteOn message is received
  //! \param msg_   The NoteOn message
  virtual void onNoteOn(NoteOn msg_)
  {
  }

  //! The callback function invoked when a PolyPressure message is received
  //! \param msg_   The PolyPressure message
  virtual void onPolyPressure(PolyPressure msg_)
  {
  }

  //! The callback function invoked when a ControlChange message is received
  //! \param msg_   The ControlChange message
  virtual void onControlChange(ControlChange msg_)
  {
  }

  //! The callback function invoked when a ProgramChange message is received
  //! \param msg_   The ProgramChange message
  virtual void onProgramChange(ProgramChange msg_)
  {
  }

  //! The callback function invoked when a ChannelPressure message is received
  //! \param msg_   The ChannelPressure message
  virtual void onChannelPressure(ChannelPressure msg_)
  {
  }

  //! The callback function invoked when a PitchBend message is received
  //! \param msg_   The PitchBend message
  virtual void onPitchBend(PitchBend msg_)
  {
  }

  //! The callback function invoked when a MIDI beat clock message is received
  //! \param msg_   The Clock message
  virtual void onClock(Clock msg_)
  {
  }

  //! The callback function invoked when a non-universal SysEx message is
  //! received
  //! \param msg_   The SysEx message
  virtual void onSysEx(SysEx msg_)
  {
  }

  //! The callback function invoked when a Universal Non-Realtime SysEx message
  //! is received
  //! \param msg_   The USysExNonRT message
  virtual void onUSysExNonRT(USysExNonRT msg_)
  {
  }

  //! The callback function invoked when a Universal Realtime SysEx message is
  //! received
  //! \param msg_   The USysExRT message
  virtual void onUSysExRT(USysExRT msg_)
  {
  }

private:
  std::unique_ptr<MidiMessage> parseMidiMessage(const tRawData& data_)
  {
    size_t length = data_.size();
    if (length < 1)
    {
      return nullptr;
    }
    uint8_t status = data_[0];
    if ((status < 0x80) || (status == 0xF4) || (status == 0xF5) || (status == 0xF9)
        || (status == 0xFD))
    {
      return nullptr;
    }
    else if (status < 0xF0)
    {
      MidiMessage::Type type = static_cast<MidiMessage::Type>(status & 0xF0);
      MidiMessage::Channel channel = static_cast<MidiMessage::Channel>(status & 0x0F);

#define M_CHANNEL_MSG_0(idMsg)   \
  case MidiMessage::Type::idMsg: \
    return length > 0 ? std::unique_ptr<idMsg>(new idMsg) : nullptr;
#define M_CHANNEL_MSG_2(idMsg)   \
  case MidiMessage::Type::idMsg: \
    return length > 1 ? std::unique_ptr<idMsg>(new idMsg(channel, data_[1])) : nullptr;
#define M_CHANNEL_MSG_3(idMsg)   \
  case MidiMessage::Type::idMsg: \
    return length > 2 ? std::unique_ptr<idMsg>(new idMsg(channel, data_[1], data_[2])) : nullptr;

      switch (type)
      {
        M_CHANNEL_MSG_3(NoteOff);
        M_CHANNEL_MSG_3(NoteOn);
        M_CHANNEL_MSG_3(PolyPressure);
        M_CHANNEL_MSG_3(ControlChange);
        M_CHANNEL_MSG_2(ProgramChange);
        M_CHANNEL_MSG_2(ChannelPressure);
        M_CHANNEL_MSG_3(PitchBend);
        default:
          return nullptr;
      }
#undef M_CHANNEL_MSG_2
#undef M_CHANNEL_MSG_3
    }
    else
    {
      MidiMessage::Type type = static_cast<MidiMessage::Type>(status);
      if (type == MidiMessage::Type::Reserved_0 || type == MidiMessage::Type::Reserved_1
          || type == MidiMessage::Type::Reserved_2
          || type == MidiMessage::Type::Reserved_3)
      {
        return nullptr;
      }
      switch (type)
      {
        case MidiMessage::Type::SysEx:
        {
          if ((length > 2 && data_[1] < 0x7E) || (length > 4 && data_[1] == 0x7E)
              || (length > 5 && data_[1] == 0x7F))
          {
            return std::unique_ptr<SysEx>(new SysEx(data_));
          }
        }
        case MidiMessage::Type::TimingClock:
        {
          return std::unique_ptr<MidiMessage>(
            new MidiMessageBase<MidiMessage::Type::TimingClock>(tRawData()));
        }
        case MidiMessage::Type::Start:
        {
          return std::unique_ptr<MidiMessage>(
            new MidiMessageBase<MidiMessage::Type::Start>(tRawData()));
        }
        case MidiMessage::Type::Continue:
        {
          return std::unique_ptr<MidiMessage>(
            new MidiMessageBase<MidiMessage::Type::Continue>(tRawData()));
        }
        case MidiMessage::Type::Stop:
        {
          return std::unique_ptr<MidiMessage>(
            new MidiMessageBase<MidiMessage::Type::Stop>(tRawData()));
        }
        default:
          return nullptr;
      }
    }
  }
};
/** @} */ // End of group Utilities
/** @} */ // End of group Midi

//--------------------------------------------------------------------------------------------------

#undef M_MIDI_BYTE
#undef M_UINT8

} // midi
} // sl
