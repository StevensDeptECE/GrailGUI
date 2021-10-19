/**
  Perform ECDH with libECC!
  This src code is given as is without ANY WARANTY.
  use at your own risk, but do wathever you want with.

  I had big headeach trying to make it work, so I thougth it may
  worth it to give if to the internet.


  ECDH : Elliptic-curve Diffie–Hellman
  ------------------------------------
  let's say Alice and Bob (ho surprise) want to talk 
  on a public channel.

  pB: private key of bob (a 'random' big number )
  pA: private key of Alice (a 'random' big number )

  pubB: public key  of BOB   (the resulting )
  pubA: public key  of ALice (the resulting )

  G: the generator point specified by the parametre of the chossen curve

  Bob compute his public key by doing  pubB=pB*G  
  Alice compute his public key by doing pubA=pA*G

  Bob and Alice echange their public key. and multiply by theit private key:
  we have thus : 

  finalPointBob = pB*pubA = pB*pA*G
  finalPointAlice = pA*pubB = pA*pB*G=pB*pA*G

  In general, we only use the X coordinate of the resulting point :)

  !!!IMPORTANT!!!
  ---------------
  never use the result of ECDH exchange directly!  use HKDF after 
  to generate the final key.  First this allow you to generate
  bigger key, second, this prevent some attacks.


  M.G.
**/
#include <stdio.h>
#include "libec.h"
#define MAX_BUFFER_SIZE 256
#define SIZE_ONE_PTS BYTECEIL(curve_params.ec_fp.p_bitlen)

/**
  Transform an affine coordinate in projection one.
  Let me explain a little : 
  to optimise calculus by preventing division, mathematiciens
  has found that passing in a 3D domain will help a lot.

  we call coordinate in this domain projected coordinate. 
  how to project ?
  {x,y}->{X,Y,Z}    simply by selecting a z != 0 and then doing
  {x,y}->{xz,yz,z}  since it work with any z!=0, I'll use z =1

  btw maybe libECC can perform this operation but didn't see where :(

  return -1 if point not on curve
  return 0 else
**/
int public_key_to_prj_pt( prj_pt_t out, fp_src_t x,
                                        fp_src_t y,
                            ec_params  *curve_params)
{
  uint8_t one[1]={1};
  fp z;
  fp_init_from_buf(&z, &(curve_params->ec_fp), one,1);

  if(is_on_curve(x, y, &(curve_params->ec_curve)))
  {
    prj_pt_init_from_coords(out, &(curve_params->ec_curve), x, y, &z);
    return 0;
  }
  return -1;
}

/**
  given a public key X,Y buffer, we will return a point (projected)
  on the curve

  return -1 if point not on curve
  return 0 else
**/
int prj_pt_from_buffer(prj_pt_t out, 
              const uint8_t * x_bfr, 
              const uint8_t * y_bfr,
           ec_params  *curve_params)
{
  fp x,y;
  fp_init_from_buf(&x,&(curve_params->ec_fp),
        x_bfr,BYTECEIL(curve_params->ec_fp.p_bitlen));
  fp_init_from_buf(&y,&(curve_params->ec_fp),
        y_bfr,BYTECEIL(curve_params->ec_fp.p_bitlen));
  return public_key_to_prj_pt(out, &x,&y,curve_params);
}


/**
  actually perform the ECDH given the public projected point 
  and the privateKey

  !!!IMPORTANT!!!
  never use the result of this exchange directly!  use HKDF after 
  to generate the final key.  First this allow you to generate
  bigger key, second, this prevent some attacks.

**/
void performECDH(fp_t sharedSecret, nn_t privKey, prj_pt_t pubKey)
{

  //3) multiply the public key by our private number ...
  prj_pt Q;
  aff_pt Q_aff;
  prj_pt_mul_monty(&Q, privKey, pubKey);
  //4) exctract the X coordinate
  prj_pt_to_aff(&Q_aff, &Q);
  fp_copy(sharedSecret, &(Q_aff.x));
}



/**
  used only for this example, 
  thoses buffer should be filled with certificate/pre established value.

  will generate a private key , fill buffer with 
  then generate the public key and fill buffer too
**/
void generate_key(uint8_t* priv_bfr, uint8_t * x_bfr, uint8_t * y_bfr,ec_params  *curve_params)
{
  nn priv;
  prj_pt pub; // public point inside the 3D plans
  aff_pt pub_aff; // remember we want point in 2D plans not 3D one
  // private 
  nn_init(&priv, 0);
  nn_get_random_mod(&priv, &(curve_params->ec_gen_order));
  // public 
  prj_pt_mul_monty(&pub, &priv, &(curve_params->ec_gen));
  prj_pt_to_aff(&pub_aff, &pub);
  // fill buffers 
  nn_export_to_buf(priv_bfr, BYTECEIL(curve_params->ec_fp.p_bitlen), &priv);
  fp_export_to_buf(x_bfr, BYTECEIL(curve_params->ec_fp.p_bitlen), &(pub_aff.x));
  fp_export_to_buf(y_bfr, BYTECEIL(curve_params->ec_fp.p_bitlen), &(pub_aff.y));

}




int main()
{
  printf("---------- start the test ------------ \n");

  /**
    Generally key are exchanged in a standard way,
    But for the example, we will regenerate wath we get if we use
    for example PEM format to key exchange. this specify :
    - the curve we use 
    - one public/private key 
    here we will simulate this, since libECC have no PEM parser ;)

  -------------------------------------------------------------------------
                        GENERATING "CERTIFICATS"
  -------------------------------------------------------------------------
  **/
  // all these info should be extracted from a standard way to
  // exchange key (ie: PEM X509, DEM, ..) since we don't have parser,...
  // we generate them ..
  const char * curve_name =  "SECP384R1" ;  // the curve we will use 
  const ec_str_params *the_curve_const_parameters; //stock const info about the curve
  ec_params  curve_params;// stock info about the curve in structured way.

  uint8_t pB[MAX_BUFFER_SIZE]={0}; //buffer bigger thant wath we need ... prevent a malloc
  uint8_t pA[MAX_BUFFER_SIZE]={0}; 

  // one should ratther use only one buffer for X,Y since we know the size 
  // for this example, we will split all 
  uint8_t pubB_x[MAX_BUFFER_SIZE]={0};
  uint8_t pubB_y[MAX_BUFFER_SIZE]={0};
  uint8_t pubA_x[MAX_BUFFER_SIZE]={0};
  uint8_t pubA_y[MAX_BUFFER_SIZE]={0};

  // fetch information pedetermined by the library. for the selected curve
  the_curve_const_parameters = ec_get_curve_params_by_name(curve_name,
                          (u8)local_strnlen((const char *) curve_name, 
                                               MAX_CURVE_NAME_LEN)+ 1);
  import_params(&curve_params, the_curve_const_parameters);
  if(MAX_BUFFER_SIZE < SIZE_ONE_PTS) // SIZE_ONE_PTS depend on the curve you have selected via const char * curve_name
  {
    printf("ERROR: select an other curve or increase MAX_BUFFER_SIZE define\n");
    return -1;
  }

/**
  generate pB, pA, pubB and pubA
**/
  generate_key(pA, pubA_x, pubA_y, &curve_params); // generate ALice priv/pub key
  generate_key(pB, pubB_x, pubB_y, &curve_params); // generate Bob priv/pub key
// ok we have all what we need to perform:



/**
  from here we return in a more "standard" way ;) 
  we had certificate that were stocked into uint8_t buffer after parsing
  we have one selected curve defined by 'curve_params'
  we can perform the ECDH


  -------------------------------------------------------------------------
                       PERFORMING ECDH Bob -> Alice
  -------------------------------------------------------------------------
**/
// all this actions are performed on the Alice's side only
nn privA;
prj_pt pubB;
fp shared_AliceSide; // the shared secret seen by alice
// loading private key from "certificatee"
nn_init_from_buf(&privA, pA, SIZE_ONE_PTS);


if(prj_pt_from_buffer(&pubB, pubB_x, pubB_y, &curve_params ))
{
    printf("ERROR: public Key is not on the curve %s",curve_name); 
    return -2;
}

performECDH(&shared_AliceSide, &privA, &pubB);


/**
  -------------------------------------------------------------------------
                       PERFORMING ECDH Alice -> Bob
  -------------------------------------------------------------------------
**/
// all this actions are performed on the Alice's side only
nn privB;
prj_pt pubA;
fp shared_BobSide; // the shared secret seen by alice
// loading private key from "certificatee"
nn_init_from_buf(&privB, pB, SIZE_ONE_PTS);
if(prj_pt_from_buffer(&pubA, pubA_x, pubA_y, &curve_params ))
{
    printf("ERROR: public Key is not on the curve %s",curve_name); 
    return -2;
}

performECDH(&shared_BobSide, &privB, &pubA);

/**
  -------------------------------------------------------------------------
                   ENDING 
  -------------------------------------------------------------------------
in practive, this section don't exist,  we perform a HKDF to 
increase the security and the size of the shared secret.
but since here we have bob's and alice's side.
we can check that they both have the same secret.
**/


fp_print("Alice secret: ",&shared_AliceSide);
fp_print("  Bob secret: ",&shared_BobSide);

return fp_eq_or_opp(&shared_AliceSide,&shared_BobSide)? 0:-3;
}