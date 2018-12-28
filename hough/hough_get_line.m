% 读取matlab自带的图像gantrycrane，并提取边缘
%RGB = imread('test.jpg');
RGB = imread('gantrycrane.png');
I  = rgb2gray(RGB);
BW = edge(I,'canny'); %提取边缘
subplot(1,2,1);imshow(RGB);title('原始图像'); 
subplot(1,2,2);imshow(BW);title('边缘图像');

% 对原始图像边缘进行hough变换，指定参数的分别率（取样间隔），区间利用默认值
[H,T,R] = hough(BW,'RhoResolution',0.5,'ThetaResolution',0.5);

% 显示hough变换的结果
figure;
imshow(imadjust(mat2gray(H)),'XData',T,'YData',R,...
   'InitialMagnification','fit');
title('Limited Theta Range Hough Transform of Gantrycrane Image');
xlabel('\theta'), ylabel('\rho');
axis on, axis normal;
colormap(hot)

% 显示霍夫变换矩阵中的极值点
P = houghpeaks(H,10,'threshold',ceil(0.3*max(H(:)))); % 从霍夫变换矩阵H中提取50个极值点
x = T(P(:,2));%极值点的theta值，即P的第二列存放的是极值点的theta值
y = R(P(:,1));%极值点的rho值，即P的第二列存放的是极值点的rho值
hold on;plot(x,y,'s','color','black');

% 找原图中的直线
lines = houghlines(BW,T,R,P,'FillGap',18,'MinLength',8);

% 绘制提取得到的直线
figure, imshow(I), hold on
max_len = 0;
for k = 1:length(lines)
    % 绘制第k条直线
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

    % 绘制第k条直线的起点（黄色）、终点（红色）
    plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
    plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');

    % 计算第k条直线的长度，保留最长直线的端点
    len = norm(lines(k).point1 - lines(k).point2);
    if ( len > max_len)
        max_len = len;
        xy_long = xy;
    end
end
title('提取到的直线');