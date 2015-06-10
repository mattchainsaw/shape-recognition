function out = visualize(file, data)
    poly = importdata(file);
    med  = importdata(data);


    % plot the poly in black
    x = poly(:,1);
    y = poly(:,2);
    fill(x,y,'w');
    hold on;

    % plot medial axis in red
    xx = [med(:,1), med(:,3)]';
    yy = [med(:,2), med(:,4)]';
    plot(xx,yy,'-r');
    
    out = true;
end
