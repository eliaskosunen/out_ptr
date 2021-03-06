# boost.out_ptr

`boost.out_ptr` is a simple parameter wrapper for output pointers.

| Appveyor | Travis CI |
| -------- | --------- |
|          |           |
|          |           |
|          |           |

# Quick Comparison and Example

This example comes from real world code using [libavformat](http://git.videolan.org/?p=ffmpeg.git;a=blob;f=libavformat/avformat.h;h=fdaffa5bf41b6ed83fa4f7acebcf04ed796296fd;hb=refs/heads/master).

<table>
  <tr>
    <td>Shared Code</td>
  </tr>
  <tr>
    <td><pre lang="cpp">
#include &lt;memory&gt;
#include &lt;avformat.h&gt;
// Signature from libavformat:
// int avformat_open_input(AVFormatContext **ps, const char *url, 
//	AVInputFormat *fmt, AVDictionary **options);

struct AVFormatContextDeleter {
		void operator() (AVFormatContext* c) noexcept {
			avformat_close_input(&c);
			avformat_free_context(c);
		}
};

using AVFormatContext = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>;
    </pre><br></td>
  </tr>
  <tr>
    <td>Current Code</td>
  </tr>
  <tr>
    <td><pre lang="cpp">
int main (int, char* argv[]) {
     AVFormatContext context(avformat_alloc_context());
     // ...
     // used, need to reopen
     AVFormatContext* raw_context = context.release();
     if (avformat_open_input(&raw_context, 
          argv[0], nullptr, nullptr) != 0) {
          std::stringstream ss;
          ss << "ffmpeg_image_loader could not open file '"
               << path << "'";
     throw FFmpegInputException(ss.str().c_str());
     }
     context.reset(raw_context);

     // ... off to the races !

     return 0;
}
    </pre><br></td>
  </tr>
  <tr>
    <td>With boost.out_ptr !</td>
  </tr>
    <td><pre lang="cpp">
int main (int, char* argv[]) {
     AVFormatContext context(avformat_alloc_context());
     // ...
     // used, need to reopen

     if (avformat_open_input(std::inout_ptr(context), 
          argv[0], nullptr, nullptr) != 0) {
          std::stringstream ss;
          ss << "ffmpeg_image_loader could not open file '"
               << argv[0] << "'";
          throw FFmpegInputException(ss.str().c_str());
     }


     // ... off to the races!

     return 0;
}
    </pre><br></td>
  </tr>
</table>

# Full Examples and Documentation

There are examples and documentation contained in this repository: please, peruse them as much as you need to! Some interesting/illuminating ones:

- It works with [custom unique pointers just fine](examples/source/std.custom_unique_ptr.cpp)
- It is [customizable to your own pointer types](examples/source/custom.handle.cpp), if you need performance or different semantics
- It works with [Boost](examples/source/boost.shared_ptr.cpp) and [Standard](examples/source/std.shared_ptr.cpp) shared pointers.
- 
