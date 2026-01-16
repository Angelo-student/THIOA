function create_tsp_file(out_file, data)
    coordinates = 1000*data;

    fid = fopen(out_file, 'w');
    
    if fid == -1
        error('无法创建文件: %s', out_file);
    end
    
    fprintf(fid, 'NAME : example_tsp\n');
    fprintf(fid, 'TYPE : TSP\n');
    fprintf(fid, 'DIMENSION : %d\n', size(coordinates, 1));
    fprintf(fid, 'EDGE_WEIGHT_TYPE : EUC_2D\n');
    fprintf(fid, 'NODE_COORD_SECTION\n');
    
    % 写入每个节点的编号和坐标
    for i = 1:size(coordinates, 1)
        fprintf(fid, '%d %.6f %.6f\n', i, coordinates(i, 1), coordinates(i, 2));
    end
    
    fprintf(fid, 'EOF\n');
    fclose(fid);
end

