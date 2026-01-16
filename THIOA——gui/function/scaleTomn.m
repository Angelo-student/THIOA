function data_scaled = scaleTomn(data,m,n)
    % SCALETOmn 将二维数据线性放缩到[-m, m]区间
    %   data_scaled = SCALETO20(data) 接收n行2列的矩阵data，返回放缩后的矩阵
    %   其中x坐标（第一列）和y坐标（第二列）分别被线性映射到[-m, m],[-n, n]区间
    
    % 输入验证
    if size(data, 2) ~= 2
        error('输入必须是n行2列的矩阵');
    end
    if isempty(data)
        error('输入矩阵不能为空');
    end
    
    % 提取x和y坐标
    x = data(:, 1);
    y = data(:, 2);
    
    % 计算x坐标的最小值和最大值
    x_min = min(x);
    x_max = max(x);
    
    % 计算y坐标的最小值和最大值
    y_min = min(y);
    y_max = max(y);
    
    if x_max == x_min
        % 所有x值相同，映射到区间中点0
        x_scaled = zeros(size(x));
    else
        % 通用线性变换公式
        x_scaled = (x - x_min) / (x_max - x_min) * 2*m - m;
    end
    
    if y_max == y_min
        y_scaled = zeros(size(y));
    else
        y_scaled = (y - y_min) / (y_max - y_min) * 2*n - n;
    end

    data_scaled = [x_scaled, y_scaled];
end
