function [hamming_distance, jk, sk] = Hamming(X_alpha, X_i)
    % 检查输入是否长度一致
    if length(X_alpha) ~= length(X_i)
        error('输入的两个向量长度不一致！');
    end

    % 初始化输出变量
    jk = [];  % alpha 差异值集合
    sk = [];  % 差异位置集合

    % 计算汉明距离和记录差异
    hamming_distance = 0; % 汉明距离初始化为 0
    for n = 1:length(X_alpha)
        if X_alpha(n) ~= X_i(n)
            hamming_distance = hamming_distance + 1; % 累加汉明距离
            jk = [jk, X_alpha(n)]; % 记录 alpha 的差异值
            sk = [sk, n];          % 记录差异发生的位置
        end
    end
end

