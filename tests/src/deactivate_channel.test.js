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
// nano_models.forEach(function(model) {
//   test('[Nano ' + model.letter + '] Swap Exact Eth For Tokens with beneficiary', zemu(model, async (sim, eth) => {

//   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xb27a69cd3190ad0712da39f6b809ecc019ecbc319d3c17169853270226d18a8a
//   const serializedTx = txFromEtherscan("0x02f9015a0181d38459682f0085215d7c1e598302a4e9947a250d5630b4cf539739df2c5dacb4c659f2488d88016345785d8a0000b8e47ff36ab50000000000000000000000000000000000000000000000018b1dd9dc51b5a9f7000000000000000000000000000000000000000000000000000000000000008000000000000000000000000015557c8b7246c38ee71ea6dc69e4347f5dac210400000000000000000000000000000000000000000000000000000000615336100000000000000000000000000000000000000000000000000000000000000002000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc20000000000000000000000006b3595068778dd592e39a122f4f5a5cf09c90fe2c001a089c5ce4ce199f7d93ea1d54c08133fab9407d8de63a9885d59d8ce69a59573dda045f6a0e0d3288dfdfddc23ad0afb9577c4011801f598d581a46cd0b0e2bd0571");

//   const tx = eth.signTransaction(
//     "44'/60'/0'/0",
//     serializedTx,
//   );

//   const right_clicks = model.letter === 'S' ? 12 : 6;

//   // Wait for the application to actually load and parse the transaction
//   await waitForAppScreen(sim);
//   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
//   await sim.navigateAndCompareSnapshots('.', model.name + '_swap_exact_eth_for_tokens_with_beneficiary', [right_clicks, 0]);

//   await tx;
//   }));
// });

// Test from constructed transaction
// EDIT THIS: build your own test
nano_models.forEach(function(model) {
  test('[Nano ' + model.letter + '] Deactivate Channel', zemu(model, async (sim, eth) => {
  const contract = new ethers.Contract(EPNSCoreContractAddr, abi);

  // 50 DAI
  const roundedNumber = 120;
  const amountOut = ethers.utils.parseUnits(roundedNumber.toString(), 18);

  const {data} = await contract.populateTransaction.deactivateChannel(amountOut);

  // Get the generic transaction template
  let unsignedTx = genericTx;
  // Modify `to` to make it interact with the contract
  unsignedTx.to = EPNSCoreContractAddr;
  // Modify the attached data
  unsignedTx.data = data;
  // EDIT THIS: get rid of this if you don't wish to modify the `value` field.
  // Modify the number of ETH sent 
  // unsignedTx.value = parseEther("0.1");

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
  await sim.navigateAndCompareSnapshots('.', model.name + '_deactivate_channel', [right_clicks, 0]);

  await tx;
  }));
});
