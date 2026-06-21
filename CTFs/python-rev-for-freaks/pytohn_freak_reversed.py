#let's reverse!

import ast

def l_reversed(x):
    x //= 0xE635_9A60
    x ^= 0xC0FFEE
    x -= 0xDEAD_BEEF
    
    return x

def m_reserved(x):
    result = []
    x_len = len(x) - 1

    for x_idx, x_elem in enumerate(x):
        l1, l2 = x_elem[0], x[x_len - x_idx][1]
        l1 = l_reversed(l1) // 6
        l2 = l_reversed(l2) // 2

        assert l1 == l2
        result.append(l1)

    return result

def n_reversed(x):
    list_as_str = x.split('=')[2]
    extracterd_list = ast.literal_eval(list_as_str)
    return extracterd_list

def prepare_input_reversed(k_reversed):
    out_str_chunk = ""

    for _ in range(8):
        curr_leter = k_reversed & 0x7F 
        if curr_leter == 0x7F:
            break

        k_reversed >>= 7
        out_str_chunk += chr(curr_leter)
    
    return out_str_chunk

def rsa_decrypt(n):
    m = (n ** 331357922622589629) % (2**59 - 1)
    return m

#helpers - no need to implement crypto
def reverse_rsa_pubkey():
    n = 2**59 - 1
    #http://www.factordb.com/index.php?query=576460752303423487 <- from n
    p, q = 179951, 3203431780337
    phi = (p - 1) * (q - 1)
    e = 420_69
    #https://www.extendedeuclideanalgorithm.com/calculator.php?mode=1&a=576457548871463200&b=42069#num <- from phi and e 
    x = -245099626248873571
    d = x % phi
    return p, q, d, n, e # -> (179951, 3203431780337, 331357922622589629, 576460752303423487, 42069)

def rsa_decrypt_helper(n):
    #https://www.dcode.fr/rsa-cipher <- used do decrypt
    resolved_hashes = {602816046677732626: 72057594037894484, 298608509588668418:29777244051795950, 492319924258423028: 27480917037021748, 487582897162708488:53783139771219121, 397152439243482845: 61193963799592159, 420883477912243645: 67205173222290672}
    return resolved_hashes[n]

looked_str = "=list(zip([l(i + 5*i) for i in x], [l(i + i) for i in x][::-1]))[::-1] = [(13969439442922757926633137632, 3251133470245911671632840864), (6919844817045365871489845728, 3067821989026578174692487328), (11408842561461143227463443808, 3766356150094573135206359136), (11299068421490417286376379488, 3802947530149782083826679648), (9203465938188223031329433888, 2306614948612889330244181216), (9753400381846729757945770272, 4656479823873291748257812704)]=="
encrypted_input_data = m_reserved(n_reversed(looked_str))
decrypted_input_data = str.join("", map(lambda x: prepare_input_reversed(rsa_decrypt_helper(x)), reversed(encrypted_input_data)))
print(decrypted_input_data) # -> ping{W0w_y0U_rYl1y_g0oD_4t_Py7h0n_c0nGr4Tz}
