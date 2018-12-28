%卡尔曼滤波器
%made by lyoid an
%条件
%%%房间内连续两个时刻温度差值的标准差为0.02度
%%%温度计的测量误差的标准差为0.5度
%%%房间内温度真值24度
%%%初始温度估计为23.5度，误差的方差为1
clc;
clear all;
QT = 0.02;
QM = 0.5;
TT = 24;
Tinit = 23.5;
TinitQ = 1;
n_iter = 100;
size = 100;


X = zeros(size,1);%观测值
A = 1;%系统参数
T =  zeros(size,1);%预测值
B = 0;%控制输入系统参数
U = zeros(size,1);%控制量
W =  zeros(size,1);%预测误差
P =  zeros(size,1);%协方差
Z =  zeros(size,1);%测量值
H =1;%测量系统参数
V =  zeros(size,1);%测量系统误差
Kg = zeros(size,1);%kalman增益
%初值
T(1) = 23.5;%预测值
X(1) = 0;%测量值
P(1) = 1;
Q = 0.02 * 0.02;%预测误差方差
R = 0.5 * 0.5;%测量误差方差

%首先，我们先要引入一个离散控制过程的系统。该系统可用一个线性随机微分方程（Linear Stochastic Difference equation）来描述：
%X(k)=A X(k-1)+B U(k)+W(k)
%再加上系统的测量值：
%Z(k)=H X(k)+V(k)
%上两式子中，X(k)是k时刻的系统状态，U(k)是k时刻对系统的控制量。A和B是系统参数，对于多模型系统，他们为矩阵。Z(k)是k时刻的测量值，H是测量系统的参数，对于多测量系统，H为矩阵。W(k)和V(k)分别表示过程和测量的噪声。他们被假设成高斯白噪声(White Gaussian Noise)，他们的协方差(covariance)分别是Q，R（这里我们假设他们不随系统状态变化而变化）。
%房间当前温度真实值为24度，认为下一时刻与当前时刻温度相同，误差为0.02度（即认为连续的两个时刻最多变化0.02度）。
for k = 2:n_iter
    %X(k|k-1)=A X(k-1|k-1)+B U(k) + W(k).. (1)
    %系统预测值，这里预测值默认不变
    T(k) = A * T(k-1) + B * U(k) + W(k);
    Z(k) = H * T(k) + V(k);
    Z(k) = 24;%这里测量值已知，恒为24
    %上以状态的最优结果的协方差传递
    %P(k|k-1)=A P(k-1|k-1) A’+Q ……… (2)
    P(k) = A * P(k-1) * A' + Q;
    %时刻T2的最优化估计值
    %X(k|k)= X(k|k-1)+Kg(k) (Z(k)-H X(k|k-1)) ……… (3)
    %其中Kg = Kg(k)= P(k|k-1) H’ / (H P(k|k-1) H’ + R) ……… (4)
    Kg(k) = P(k) * H' / (H *  P(k) * H' + R);
    T(k) = T(k) + Kg(k) * (Z(k) - H * T(k));
    %到现在为止，我们已经得到了k状态下最优的估算值X(k|k)。但是为了要令卡尔曼滤波器不断的运行下去直到系统过程结束，我们还要更新k状态下X(k|k)的协方差：
    %P(k|k)=（I-Kg(k) H）P(k|k-1) ……… (5)
    P(k) = (1 - Kg(k) * H) * P(k);
end
%作图
FontSize=14;
LineWidth=3;
figure();
%最优温度估计
plot(T,'b-','LineWidth',LineWidth) %画出最优估计值
hold on;