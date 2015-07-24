function out = visualize(file, data, prun)
    poly = importdata(file);
    med  = importdata(data);
    path = importdata(prun);

    % plot shape with medial axis
    figure;
    subplot(2,2,1);
    % plot the poly with black boundary
    x = poly(:,1);
    y = poly(:,2);
    fill(x,y,'w');
    hold on;
    % plot medial axis in red
    xx = [med(:,1), med(:,3)]';
    yy = [med(:,2), med(:,4)]';
    plot(xx,yy,'-r');
    % show center
    plot(path(1,1),path(1,2),'ok');
    title('Medial Axis');

    % plot pruning output
    subplot(2,2,2);
    fill(x,y,'w');
    hold on;
    % plot branches in solid blue
    xxx = [path(:,1), path(:,3)]';
    yyy = [path(:,2), path(:,4)]';
    plot(xxx,yyy,'-b');
    title('Pruning');

    % plot EDF data
    subplot(2,2,3);
    fill(x,y,'w');
    hold on;
    plot(xx,yy,'-r');
    % EDF (Black) and radii (blue)
    EDF_x = med(:,1);
    EDF_y = med(:,2);
    EDF_val = num2str(med(:,5));
    text(EDF_x, EDF_y, EDF_val);
    title('EDF Data');
    
    % plot radii
    subplot(2,2,4);
    R = med(:,6);
    drawCircle(EDF_x,EDF_y, R); %Just use the same x and y
    title('Radii');


    figure;
    fill(x,y,'w');
    hold on;
    plot(xx,yy,'-r');
    text(EDF_x, EDF_y, EDF_val);
    title('EDF Data');
    
    out = true;
end
