% plots a circle with radius R at (X,Y) in blue.
function out = drawCircle(X,Y,R)
    t = linspace(0, 2*pi, 100);
    cot = cos(t);
    sit = sin(t);
    for i = 1:length(X)
        xt = X(i) + R(i) * cot;
        yt = Y(i) + R(i) * sit;
        hold on;
        plot(xt, yt, 'b');
    end
    out = true;
end