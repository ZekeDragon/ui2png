# The Qt UI to PNG Renderer Tool

This tool was designed to be a simple command line tool that would take a given Qt .ui file that describes a widget or window, and output a rendering of that widget as a PNG file. You can use this through your terminal or Powershell prompt, by calling `ui2png some_ui_file.ui` and it will create the file **some_ui_file.ui.png** in the same folder as *some_ui_file.ui* is located. By default, it outputs at 640x480 widget size, but you can set the size by using the '-s' option, for example `ui2png -s 800x600 some_ui_file.ui`. You can change the output file by adding the optional destination filename after the source. If a file already exists at the destination location, ui2png will stop and not overwrite it, unless provided the '-f' flag when run.

## Installation

For the simplest uses, on Windows just download the released zip file and extract it wherever you would prefer. All the libraries necessary for running on Windows 10 or 11 should be included. You'll need to use cmd or Windows Powershell to use it, just call "ui2png -h" for use instructions. For Linux or Mac, you'll need to build from source, and I haven't really tested it, but I can't see any reason it wouldn't work.

This hasn't been worked on much, so don't look at it as anything more than purely experimental.

