
#include <nan.h>
using namespace v8;

#include <string>
#include <sstream>
#include "speex/speex.h"
/* 目前支持的原始音频格式为
单声道（mono）
采样频率:11025
采样格式:PCM16

使用流程:
1、发送音频时的编码
AudioFormatTool audio_tool = AudioFormatTool(msg_version, audio_ori, false);
std::string for_send = audio_tool.GetEncodedString();

2接收音频时的解码
AudioFormatTool audio_tool = AudioFormatTool(msg_version, audio_encoded, true);
std::string for_play = audio_tool.GetPCMString();
*/
const uint32_t AFT_RATE = 11025;
class  AudioFormatTool{
public:
    AudioFormatTool(uint32_t version, const std::string& audio, bool encoded);
    double GetSec();
    std::string GetPCMString();
    std::string GetEncodedString();
private:
    std::string Encode(const std::string& audio, uint32_t version);
    std::string Decode(const std::string& audio, uint32_t version);
private:
    std::string audio_pcm_;
    std::string audio_encoded_;
};


AudioFormatTool::AudioFormatTool(uint32_t version, const std::string &audio, bool encoded){
    if(version == 0){
        audio_pcm_ = encoded?Decode(audio, version):audio;
        audio_encoded_ = encoded?audio:Encode(audio, version);
    }
}

double AudioFormatTool::GetSec(){
    return audio_pcm_.size()*1.0/2/AFT_RATE;
}

std::string AudioFormatTool::GetPCMString(){
    return audio_pcm_;
}

std::string AudioFormatTool::GetEncodedString(){
    return audio_encoded_;
}

std::string AudioFormatTool::Encode(const std::string &audio, uint32_t version){
    SpeexBits enc_bits;
    int enc_frame_size = 0;
    void *enc_state;
    std::string audio_in = audio;
    //init
    speex_bits_init(&enc_bits);
    enc_state = speex_encoder_init(&speex_nb_mode);
    int quality = 8;
    speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &quality);
    speex_encoder_ctl(enc_state, SPEEX_GET_FRAME_SIZE, &enc_frame_size);

    //encode
    std::string rtn;
    int frames_count = audio_in.size()/2/enc_frame_size+1;
    std::string for_fill(enc_frame_size*2, 0);
    audio_in+=for_fill;
    for(int i = 0; i < frames_count; i++) {
        speex_bits_reset(&enc_bits);
        speex_encode_int(enc_state, (int16_t*)audio_in.data()+i*enc_frame_size, &enc_bits);
        int count = speex_bits_nbytes(&enc_bits);
        std::string frame_buf;
        frame_buf.resize(count+1, 0);//add a byte to save length
        (*(uint8_t*)frame_buf.data())=count;
        speex_bits_write(&enc_bits, (char *)frame_buf.data()+1, count+100);
        rtn += frame_buf;
    }
    return rtn;
}

std::string AudioFormatTool::Decode(const std::string &audio, uint32_t version)
{
    int dec_frame_size;
    SpeexBits dec_bits;
    void *dec_state;

    //init
    speex_bits_init(&dec_bits);
    dec_state = speex_decoder_init(&speex_nb_mode);
    speex_decoder_ctl(dec_state, SPEEX_GET_FRAME_SIZE, &dec_frame_size);

    std::string rtn;
    if(audio.size() > 1){
        int index = 0;
        while(1){
            if(index >= audio.size())break;
            uint8_t frame_size = *((uint8_t*)audio.data());
            index++;
            if(index+frame_size > audio.size())break;
            speex_bits_read_from(&dec_bits, audio.data()+index, frame_size);
            std::string frame_buf;
            frame_buf.resize(dec_frame_size*2, 0);
            speex_decode_int(dec_state, &dec_bits, (int16_t*)frame_buf.data());
            rtn += frame_buf;
            index+=frame_size;
        }
    }
    return rtn;
}


std::string inline Byte2HexAsc(const std::string& bytes){
    std::string out;
    std::ostringstream o_stream;
    for(uint8_t item:bytes){
        o_stream.width(2);
        o_stream.fill('0');
        o_stream<<std::hex<<(uint32_t)item;
    }
    o_stream.flush();
    return o_stream.str();
}

std::string inline HexAsc2ByteString(const std::string& hex){
    std::string rtn;
    for(size_t i = 0; i < hex.length(); i+=2){
        rtn += (char)std::strtol(hex.substr(i, 2).c_str(), nullptr, 16);
    }
    return rtn;
}

/*

void decode(const FunctionCallbackInfo<Value>& args){
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<String> audio_in = args[0]->ToString(context).ToLocalChecked();
  std::string xxx (audio_in);
  String::Utf8Value audio_in_utf8(audio_in);
  std::string audio_in_str = std::string(*audio_in_utf8);
  std::string audio_in_byte = HexAsc2ByteString(audio_in_str);
  AudioFormatTool audio_tool = AudioFormatTool(0, audio_in_byte, true);
  std::string for_play = audio_tool.GetPCMString();
  std::string for_play_asc = Byte2HexAsc(for_play);
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, for_play_asc.c_str()));
}*/

NAN_METHOD(decode) {
  Nan::HandleScope scope;
  v8::Local<v8::Object> audio_in = info[0].As<v8::Object>();
  const unsigned char* audio_in_char = (const unsigned char*) node::Buffer::Data(audio_in);
  uint32_t lenght = node::Buffer::Length(audio_in);

  AudioFormatTool audio_tool = AudioFormatTool(0, std::string((char*)audio_in_char, lenght), true);
  std::string for_play = audio_tool.GetPCMString();
  info.GetReturnValue().Set(Nan::CopyBuffer((const char*) for_play.data(), (uint32_t) for_play.size()).ToLocalChecked());
}


NAN_MODULE_INIT(Init) {
  Nan::Export(target, "decode", decode);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)