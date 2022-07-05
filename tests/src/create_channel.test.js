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

//   const serializedTx = txFromEtherscan("0x5148982600000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000002b5e3af16b1880000000000000000000000000000000000000000000000000000000000000000003d312b6261666b726569667878717035706c35733775337733626a617332613273716c357665736532793369716f636732756d346f746b32696d626f6461");

//   const tx = eth.signTransaction(
//     "44'/60'/0'/0",
//     serializedTx,
//   );

//   const right_clicks = model.letter === 'S' ? 6 : 4;

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
  // EDIT THIS: get rid of this if you don't wish to modify the `value` field.
  // Modify the number of ETH sent
  // unsignedTx.value = parseEther("0.1");

  console.log('data: ====> ', data);

  // Create serializedTx and remove the "0x" prefix
  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    serializedTx
  );

  const right_clicks = 4;

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 10 times, then pressing both buttons to accept the transaction.
  // EDIT THIS: modify `10` to fix the number of screens you are expecting to navigate through.
  await sim.navigateAndCompareSnapshots('.', model.name + '_create_channel_with_fees', [right_clicks, 0]);

  await tx;
  }));
});
