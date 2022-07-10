import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models,SPECULOS_ADDRESS, txFromEtherscan} from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";

// EDIT THIS: Replace with your contract address
const EPNSCoreContractAddr = "0x66329fdd4042928bfcab60b179e1538d56eeeeee";
// EDIT THIS: Replace `boilerplate` with your plugin name
const pluginName = "EPNS";
const abi_path = `../${pluginName}/abis/` + EPNSCoreContractAddr + '.json';
const abi = require(abi_path);

// Test from replayed transaction: https://etherscan.io/tx/0x0160b3aec12fd08e6be0040616c7c38248efb4413168a3372fc4d2db0e5961bb
// EDIT THIS: build your own test

const [s, sp, x] = nano_models;

// [x].forEach(function(model) {
//   test('[Nano ' + model.letter + '] Create Channel With Fees', zemu(model, async (sim, eth) => {

//   const serializedTx = txFromEtherscan("0x5148982600000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000002b5e3af16b1880000000000000000000000000000000000000000000000000000000000000000003d312b6261666b726569667878717035706c35733775337733626a617332613273716c357665736532793369716f636732756d346f746b32696d626f6461000000");

//   const tx = eth.signTransaction(
//     "44'/60'/0'/0",
//     serializedTx,
//   );

//   const right_clicks = 4;

//   // Wait for the application to actually load and parse the transaction
//   await waitForAppScreen(sim);
//   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
//   await sim.navigateAndCompareSnapshots('.', model.name + '_create_channel_with_fees', [right_clicks, 0]);

//   await tx;
//   }));
// });

// Test from constructed transaction
// EDIT THIS: build your own test


[x].forEach(function(model) {
  test('[Nano ' + model.letter + '] Create Channel', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(EPNSCoreContractAddr, abi);

  const channelType = 2;
  const ipfsHash = 'bafkreidrk5fiabwv273tt2tztlua46kaeezrhv2l3mnvqvzwu6hs3nm5vi';
  const identity = "1+" + ipfsHash;
  const identityBytes = ethers.utils.toUtf8Bytes(identity);

  const channelCreationFees = 50; // 50 DAI
  const feesAmount = ethers.utils.parseUnits(channelCreationFees.toString(), 18);

  // https://etherscan.io/tx/0x66d378b151374c97800434eea221d26bf9df2de1a2e75997f56dbe4bb10993a0
  const {data} = await contract.populateTransaction.createChannelWithFees(
    channelType,
    identityBytes,
    feesAmount
  );

  // Get the generic transaction template
  let unsignedTx = genericTx;
  // Modify `to` to make it interact with the contract
  unsignedTx.to = EPNSCoreContractAddr;
  // Modify the attached data
  unsignedTx.data = data;

  // Create serializedTx and remove the "0x" prefix
  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

  console.log('------------->> TXN is about to be signed');

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = 4;

  console.log('------------->> BEFORE waitForAppScreen');

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);

  console.log('------------->> AFTER waitForAppScreen');
  await sim.navigateAndCompareSnapshots('.', model.name + '_create_channel_with_fees', [right_clicks, 0]);
  console.log('------------->> after navigateAndCompareSnapshots');
  await tx;
  }));
});
