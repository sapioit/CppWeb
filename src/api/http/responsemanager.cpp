#include <http/responsemanager.h>
#include <io/outputscheduler.h>
#include <io/datapacket.h>
#include <misc/log.h>
#include <misc/storage.h>

#include <sstream>

using namespace Http;

void ResponseBuilder::Respond(Http::Response response, IO::Socket& socket)
{
    try {
        // TODO create data packets and schedule them

        IO::DataPacket* header = new IO::MemoryPacket<std::string>{ response.str_header() };
        IO::DataPacket* body = nullptr;
        if (response.has_resource()) {
            body = new IO::File{ response.getResource().path() };
        }
        else {
            body = new IO::MemoryPacket<std::string>{ response.getText() };
        }
        IO::DataPacket* ending = new IO::MemoryPacket<std::string>{ "\r\n\r\n" };

        std::queue<IO::DataPacket*> packets;
        packets.push(header);
        packets.push(body);
        packets.push(ending);

        IO::OutputScheduler::get().ScheduleWrite(socket, header,body,ending);

        //IO::scheduled_write sched_write;
        //      case states::Body: {
        //        if (has_resource()) {
        //          std::copy(_resource.content().begin(), _resource.content().end(),
        //                    std::ostream_iterator<char>(stream));
        //        } else {
        //          stream << _text;
        //        }
        //        stream << crlf;
        //        machine.transition(transitions::EndBody);
        //        break;
        //      }
        //      case states::CRLFBody: {
        //        stream << crlf;
        //        machine.transition(transitions::CRLFEnd);
        //        break;
        //      }

        //IO::OutputScheduler::get().ScheduleWrite(socket, std::move(raw_response));
    }
    catch (std::exception& ex) {
        Log::e(ex.what());
        ResponseBuilder::Respond({ response.getRequest(), 500 }, socket);
    }
}
