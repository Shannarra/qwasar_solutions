def my_roman_numerals_converter(num)
    return '' if num == 0

    roman_numbers = {
        1000 => "M",  
        900 => "CM",  
        500 => "D",  
        400 => "CD",
        100 => "C",  
        90 => "XC",  
        50 => "L",  
        40 => "XL",  
        10 => "X",  
        9 => "IX",  
        5 => "V",  
        4 => "IV",  
        1 => "I",  
        0 => "",  
    }.freeze;

    roman_numbers.each do |value, letter|
      return ( letter * (num / value)) << my_roman_numerals_converter(num % value) if value <= num
    end
    return my_roman_numerals_converter(num % value)

end