def my_hamming_dna(a, b)
    return 0 if a == b
    return -1 if a.length != b.length
  
    dist = 0
    a.chars.each_with_index do |c, i|
      dist += 1 if c != b[i]
    end
    dist
  end
  