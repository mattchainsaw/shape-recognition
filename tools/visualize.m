function out = visualize(file, data)
    poly = importdata(file);
    med  = importdata(data);


    % plot the poly in black
    x = poly(:,1);
    y = poly(:,2);
    fill(x,y,'w');
    hold on;
    
    pause;

    % plot medial axis in red
    xx = [med(:,1), med(:,3)]';
    yy = [med(:,2), med(:,4)]';
    plot(xx,yy,'-r');
    
    pause;
    
    % EDF (Black) and radii (blue)
    EDF_x = med(:,1);
    EDF_y = med(:,2);
    EDF_val = num2str(med(:,5));
    text(EDF_x, EDF_y, EDF_val);
    
    pause;
    
    R = med(:,6);
    drawCircle(EDF_x,EDF_y, R);
    
    pause;
    
    out = true;
end
