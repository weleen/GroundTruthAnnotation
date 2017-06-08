%use imaqhwinfo() find device name
%videoinput('name',1)
function videoRead()
%camera video read
src=videoinput('dcam',1);
figure('Name','Camera')
uicontrol('String','Close','Callback','close(gcf)');

vidRes=get(src,'VideoResolution');
nBands=get(src,'NumberofBands');
hImage=image(zeros(vidRes(2),vidRes(1),nBands));
preview(src.hImage);
end