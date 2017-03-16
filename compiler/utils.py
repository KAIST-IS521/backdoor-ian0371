def printf(s, string_addr_reg, tmp_reg, string_addr):
    print("puti r{0}, {1}".format(string_addr_reg, string_addr))
    for i in s:
        print("""puti r{2}, {0}
store r{1}, r{2}
puti r{2}, 1
add r{1}, r{1}, r{2}""".format(ord(i), string_addr_reg, tmp_reg, string_addr))
    print("puti r{0}, {1}".format(string_addr_reg, string_addr))
    print("puts r{0}".format(string_addr_reg))

printf("Not enough heap memory\n", 0, 1, 0)

