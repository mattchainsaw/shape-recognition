function out = drawCircle(X,Y,R)
    % parametrization variable for circle (use N+1 as first point counts twice)
    t = linspace(0, 2*pi, 100);
    cot = cos(t);
    sit = sin(t);

    % compute discretization of each circle
    for i = 1:length(X)
        xt = X(i) + R(i) * cot;
        yt = Y(i) + R(i) * sit;
        hold on;
        plot(xt, yt, 'b');
    end
    out = true;
end
