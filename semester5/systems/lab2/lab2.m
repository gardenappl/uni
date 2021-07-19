clc
clear
close all

% Load images.

img1 = imread("x1.bmp");
img2 = imread("y3.bmp");

X = [double(img1); ones(1, size(img1, 2))];
Y = double(img2);

% Find pseudoinverse (Moore-Penrose).

delta = 10;

Xpp = X' / (X * X' + delta * eye(size(X, 1)));

while true
    delta = delta / 2;
    Xp = X' / (X * X' + delta * eye(size(X, 1)));
    if (norm(Xpp - Xp, 1) < 0.01)
        break;
    end
    Xpp = Xp;
end

% Find pseudoinverse (Greville)

Xg = X(1,:);
if all(Xg == 0)
    Xpg = zeros(length(Xg), 1);
else
    Xpg = Xg' / (Xg * Xg');
end

for i = 2:size(X, 1)
    xgnext = X(i,:);
    
    Z = eye(size(X, 2)) - Xpg * Xg;
    if xgnext * Z * xgnext' < 0.01
        R = Xpg * Xpg';
        Xpg = [
            Xpg - (R * xgnext' * xgnext * Xpg) / (1 + xgnext * R * xgnext'), ...
            (R * xgnext') / (1 + xgnext * R * xgnext')
        ];
    else
        Xpg = [
            Xpg - (Z * xgnext' * xgnext * Xpg) / (xgnext * Z * xgnext'), ...
            (Z * xgnext') / (xgnext * Z * xgnext')
        ];
    end
    Xg = [Xg; xgnext];
end

% Check correctness of pseudoinverse


% (Moore-Penrose)
if ~(norm(X * Xp * X - X, 1) < 0.1)
    error("Moore-Penrose pseudoinverse is wrong.");
elseif ~(norm(Xp * X * Xp - Xp, 1) < 0.1)
    error("Moore-Penrose pseudoinverse is wrong.");
elseif ~(norm(X * Xp - (X * Xp)', 1) < 0.1)
    error("Moore-Penrose pseudoinverse is wrong.");
elseif ~(norm(Xp * X - (Xp * X)', 1) < 0.1)
    error("Moore-Penrose pseudoinverse is wrong.");
end


% (Greville)
if ~(norm(X * Xpg * X - X, 1) < 0.1)
    error("Greville pseudoinverse is wrong.");
elseif ~(norm(Xpg * X * Xpg - Xpg, 1) < 0.1)
    error("Greville pseudoinverse is wrong.");
elseif ~(norm(X * Xpg - (X * Xpg)', 1) < 0.1)
    error("Greville pseudoinverse is wrong.");
elseif ~(norm(Xpg * X - (Xpg * X)', 1) < 0.1)
    error("Greville pseudoinverse is wrong.");
end

% Get model and show results.

A = Y * Xp;
Ag = Y * Xpg;

figure;
imshow(img1);

figure;
imshow(img2);

newimg2 = uint8(A * X);
min(min(newimg2))
max(max(newimg2))
abs(norm(double(newimg2 - img2), 1))


figure;
imshow(newimg2);

newimg2g = uint8(Ag * X);
abs(norm(double(newimg2g - img2), 1))

figure;
imshow(newimg2g);