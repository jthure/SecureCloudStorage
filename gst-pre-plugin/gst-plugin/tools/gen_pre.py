from charm.schemes.prenc.pre_afgh06_temp_jm import AFGH06Temp
from charm.core.engine.util import objectToBytes
from charm.toolbox.pairinggroup import PairingGroup

group = PairingGroup('SS512')
scheme = AFGH06Temp(group)
params = scheme.setup()

pk_a, sk_a = scheme.keygen(params)
pk_b, sk_b = scheme.keygen(params)
pk_c, sk_c = scheme.keygen(params)
pk_d, sk_d = scheme.keygen(params)

rk_ab_2018 = scheme.re_keygen(params, sk_a, pk_b, l='2018')
rk_ab_2017 = scheme.re_keygen(params, sk_a, pk_b, l='2017')
rk_ac_2018 = scheme.re_keygen(params, sk_a, pk_c, l='2018')

print("const char *params" + ' = "' + objectToBytes(params, group).decode('ascii') + '";')
for pk, sk, i in [(pk_a, sk_a, 'a'), (pk_b, sk_b, 'b'), (pk_c, sk_c, 'c'), (pk_d, sk_d, 'd')]:
    print("const char *pk_" + i + ' = "' + objectToBytes(pk, group).decode('ascii') + '";')
    print("const char *sk_" + i + ' = "' + objectToBytes(sk, group).decode('ascii') + '";')

for rk, a, b, t in [(rk_ab_2018, 'a', 'b', '2018'), (rk_ab_2017, 'a', 'b', '2017'), (rk_ab_2018, 'a', 'c', '2018')]:
    print("const char *rk_" + a + b + '_' + t + ' = "' + objectToBytes(rk, group).decode('ascii') + '";')
