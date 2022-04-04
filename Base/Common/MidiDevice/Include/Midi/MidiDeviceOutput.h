#ifndef MidiDeviceOutputH
#define MidiDeviceOutputH

#include <Midi/MidiInterfaceOutput.h>

#include "../RtMidi4/RtMidi4.h"

namespace Midi
{
   namespace Device
   {
      // connect to an existing output
      class Output : public Interface::Output
      {
      public:
         Output(const QString& outputPortName);
         virtual ~Output();

      public:
         void open() override;
         void close() override;

      protected:
         RtMidiOut output;

      private:
         void sendBuffer(const Bytes& buffer) override;
         void openOutput(bool verbose);
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);

      private:
         const std::string outputPortName; // rt midi does not use QString
      };
   } // namespace Device
} // namespace Midi

#endif // NOT MidiDeviceOutputH
