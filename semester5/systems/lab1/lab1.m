T = 5;
dt = 0.01;
t = 0:dt:T;
y = load('f4.txt');
N = length(y);

figure;
plot(t, y), grid;

fourier = abs(fft(y));


% Find maximum
f_maxima = islocalmax(fourier);
[k_max_value, k_max] = max(fourier .* f_maxima);
k_max = k_max - 1; %count from 0 instead of from 1

% only search for maximum in the first half
if (k_max > N/2)
    k_max = N - k_max;
end

figure;
plot(1:N, fourier), grid;

figure;
plot(1:round(N/2), fourier(1:round(N/2)), k_max, k_max_value, 'r*'), grid;

df = 1/T;
f_max = k_max * df;

% Least Squares method
f_sin = sin(2*pi*f_max*t);
B = [sum(t.^6), sum(t.^5), sum(t.^4), sum(f_sin.*t.^3), sum(t.^3);
     sum(t.^5), sum(t.^4), sum(t.^3), sum(f_sin.*t.^2), sum(t.^2);
     sum(t.^4), sum(t.^3), sum(t.^2), sum(f_sin.*t),    sum(t);
     sum(f_sin.*t.^3), sum(f_sin.*t.^2), sum(f_sin.*t), sum(f_sin.*f_sin), sum(N*f_sin);
     sum(t.^3), sum(t.^2), sum(t),    sum(N*f_sin),     N];
 
c = [sum(y.*t.^3), sum(y.*t.^2), sum(y.*t), sum(y.*f_sin),  sum(y)]';
a = inv(B)*c;
f_approx = a(1).*t.^3 + a(2).*t.^2 + a(3).*t + a(4).*f_sin + a(5);

figure;
plot(t, f_approx), grid;
I_error = sum((y - f_approx).^2);
