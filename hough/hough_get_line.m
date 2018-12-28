% ��ȡmatlab�Դ���ͼ��gantrycrane������ȡ��Ե
%RGB = imread('test.jpg');
RGB = imread('gantrycrane.png');
I  = rgb2gray(RGB);
BW = edge(I,'canny'); %��ȡ��Ե
subplot(1,2,1);imshow(RGB);title('ԭʼͼ��'); 
subplot(1,2,2);imshow(BW);title('��Եͼ��');

% ��ԭʼͼ���Ե����hough�任��ָ�������ķֱ��ʣ�ȡ�����������������Ĭ��ֵ
[H,T,R] = hough(BW,'RhoResolution',0.5,'ThetaResolution',0.5);

% ��ʾhough�任�Ľ��
figure;
imshow(imadjust(mat2gray(H)),'XData',T,'YData',R,...
   'InitialMagnification','fit');
title('Limited Theta Range Hough Transform of Gantrycrane Image');
xlabel('\theta'), ylabel('\rho');
axis on, axis normal;
colormap(hot)

% ��ʾ����任�����еļ�ֵ��
P = houghpeaks(H,10,'threshold',ceil(0.3*max(H(:)))); % �ӻ���任����H����ȡ50����ֵ��
x = T(P(:,2));%��ֵ���thetaֵ����P�ĵڶ��д�ŵ��Ǽ�ֵ���thetaֵ
y = R(P(:,1));%��ֵ���rhoֵ����P�ĵڶ��д�ŵ��Ǽ�ֵ���rhoֵ
hold on;plot(x,y,'s','color','black');

% ��ԭͼ�е�ֱ��
lines = houghlines(BW,T,R,P,'FillGap',18,'MinLength',8);

% ������ȡ�õ���ֱ��
figure, imshow(I), hold on
max_len = 0;
for k = 1:length(lines)
    % ���Ƶ�k��ֱ��
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

    % ���Ƶ�k��ֱ�ߵ���㣨��ɫ�����յ㣨��ɫ��
    plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
    plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');

    % �����k��ֱ�ߵĳ��ȣ������ֱ�ߵĶ˵�
    len = norm(lines(k).point1 - lines(k).point2);
    if ( len > max_len)
        max_len = len;
        xy_long = xy;
    end
end
title('��ȡ����ֱ��');