#ifndef AudioDeviceDriverH
#define AudioDeviceDriverH

#include <functional>

#include "../Private/AudioDeviceCommon.h"
#include "../portaudio/install/include/portaudio.h"

namespace AudioDevice
{
   namespace Common // some common values
   {
      static constexpr float SampleRateDefault = 0.0;
      static constexpr float SampleRateCD = 44100.0;
      static constexpr float SampleRateNormal = 48000.0;
      static constexpr float SampleRateHigh = 96000.0;

      static const QString DeviceES8 = "ES-8";
   }; // namespace Common

   // A DC coupled sound interface
   class Driver
   {      
   public:
      using AudioLoopFunction = std::function<void(const float& audioCallackRate)>;
      using InputFunction = std::function<void(const InputBuffer& inputBuffer)>;
      using OutputFunction = std::function<void(OutputBuffer& inputBuffer)>;

      struct DeviceInfo
      {
         QString name;
         bool cdSampleRate;
         bool normalSampleRate;
         bool highSampleRate;

         using List = QList<DeviceInfo>;
      };

   public:
      Driver(const QString& deviceName, float sampleRate = Common::SampleRateDefault, Frame framesPerBuffer = 0);
      virtual ~Driver();

   public:
      static DeviceInfo::List listDevices();
      float getSampleRate() const;

      void registerInputFunction(InputFunction inputFunction, const Channel& channel);
      void registerOutputFunction(OutputFunction outputFunction, const Channel& channel);

      template <typename ClassType>
      void registerAudioLoopFunction(ClassType* instance, void (ClassType::*functionPointer)(const float&));

   protected:
      virtual void internalCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer);

   private:
      using InputFunctionMap = QMap<Channel, InputFunction>;
      using OutputFunctionMap = QMap<Channel, OutputFunction>;

   private:
      static int portAudioCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
      void startStream(const PaDeviceIndex& deviceId, const float& sampleRate, const Frame& framesPerBuffer);

   private:
      static uint useCount;
      const PaDeviceInfo* device;
      PaStream* stream;

      InputFunctionMap inputFunctionMap;
      AudioLoopFunction audioLoopFunction;
      OutputFunctionMap outputFunctionMap;
   };
} // namespace AudioDevice

#include "../Private/AudioDeviceDriver.hpp"

#endif // AudioDeviceDriverH
