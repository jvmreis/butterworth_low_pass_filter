% ESPECIFICAÃ‡Ã•ES DO FILTRO
fs = 48000;             % Taxa de amostragem em Hz
Fs_Hz=fs;
stopband_freq = 3600;   % FrequÃªncia de corte da banda de rejeiÃ§Ã£o em Hz
passband_freq = 2000;   % FrequÃªncia de corte da banda passante em Hz

delta_passband = 0.01;  % TolerÃ¢ncia de ganho na banda passante
delta_stopband = 0.001; % TolerÃ¢ncia de ganho na banda de rejeiÃ§Ã£o

% NormalizaÃ§Ã£o das frequÃªncias
Wp = (passband_freq / (fs/2));
Ws = (stopband_freq / (fs/2));

ap = -20 * log10(delta_passband); % AtenuaÃ§Ã£o na banda de rejeiÃ§Ã£o em dB
as = -20 * log10(delta_stopband); % AtenuaÃ§Ã£o na banda passante em dB

[N, wc] = buttord(Wp, Ws, ap, as); % Retorna a ordem do filtro e a frequÃªncia de corte

% Projeto do filtro Butterworth
[b, a] = butter(N, wc, 'low');
FName="low";

 figure,freqz(real(b),real(a));

    % Print transfer function
    fprintf('\nFilter Transfer Function:\n');
    fprintf('ordem:%d\n', N);
    fprintf('Numerator:%s\n', mat2str(b));
    fprintf('Denominator:%s\n', mat2str(a));

    a1=mat2str(a);
    b2=mat2str(b);
    
    % Convert filter to second-order sections
    sos = tf2sos(b, a);
    
    % Print 32bits second-order transfer functions
    sos32 = single(sos);
    fprintf('\nSecond Order Sections 32bits Transfer Functions:\n');
    for Sec = 1:size(sos32, 1)
        fprintf('*Section %d\n', Sec);
        TFstr = 'H(z)=';
        for k = 1:3
            if k == 1
                TFstr = [TFstr, num2str(sos32(Sec, k))];
            else
                TFstr = [TFstr, ' + ', num2str(sos32(Sec, k)), '(z^-', num2str(k-1), ')'];
            end
        end
        TFstr = [TFstr, '------------'];
        for k = 4:6
            if k == 4
                TFstr = [TFstr, num2str(sos32(Sec, k))];
            else
                TFstr = [TFstr, ' + ', num2str(sos32(Sec, k)), '(z^-', num2str(k - 4), ')'];
            end
        end
        fprintf('%s\n', TFstr);
    end
    
    % Show Frequency response
    [wd, hd] = freqz(b, a, 1000, Fs_Hz);
    
    % Convert coeffs to 32bits
    sos32 = single(sos);
    sos32 = double(sos32);
    [b32, a32] = sos2tf(sos32);
   
    
    % Biquads - a coefs
    a = zeros(size(sos32, 1) * 2, 1, 'single');
    for Sec = 1:size(sos32, 1)
        a((Sec - 1) * 2 + 1) = sos32(Sec, 5);
        a((Sec - 1) * 2 + 2) = sos32(Sec, 6);
    end
    a_coeff = sprintf('const byte %s_a_bytes[%d]={\n\t', FName, numel(a) * 4);
    BytesCount = 0;
    for Byte = typecast(a, 'uint8')
        a_coeff = [a_coeff, sprintf('0x%02X,', Byte)];
        BytesCount = BytesCount + 1;
        if(BytesCount >= 4)
            a_coeff = [a_coeff, '\n\t']
            BytesCount = 0;
        end
    end
    a_coeff = [a_coeff(1:end-1), '};\n']

    % b coefs
    b = zeros(size(sos32, 1) * 3, 1, 'single');
    for Sec = 1:size(sos32, 1)
        b((Sec - 1) * 3 + 1) = sos32(Sec, 1);
        b((Sec - 1) * 3 + 2) = sos32(Sec, 2);
        b((Sec - 1) * 3 + 3) = sos32(Sec, 3);
    end
    b_coeff = sprintf('const byte %s_b_bytes[%d]={', FName, numel(b) * 4);
    BytesCount = 0;
    for Byte = typecast(b, 'uint8')
        b_coeff = [b_coeff, sprintf('0x%02X,', Byte)];
        BytesCount = BytesCount + 1;
        if(BytesCount >= 4)
            b_coeff = [b_coeff, '\n\t']
            BytesCount = 0;
        end
    end
    b_coeff = [b_coeff(1:end-1), '};\n']