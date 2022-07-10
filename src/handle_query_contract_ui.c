#include "boilerplate_plugin.h"

void copy_amount_with_ticker(const uint8_t *amount,
                             uint8_t amount_size,
                             uint8_t amount_decimals,
                             char *ticker,
                             uint8_t ticker_size,
                             char *out_buffer,
                             uint8_t out_buffer_size) {
    char tmp_buffer[100] = {0};
    amountToString(amount, amount_size, amount_decimals, "", tmp_buffer, 100);
    uint8_t stringLen = strnlen(tmp_buffer, sizeof(tmp_buffer)) + 1 + ticker_size;
    snprintf(out_buffer, MIN(out_buffer_size, stringLen), "%s %s", tmp_buffer, ticker);
    out_buffer[out_buffer_size - 1] = '\0';
}

static void set_add_delegate_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Delegate Address", msg->titleLength);
            
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';

            // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
            // Setting it to `0` will make it work with every chainID :)
            uint64_t chainid = 0;


            // Get the string representation of the address stored in `context->beneficiary`. Put it in
            // `msg->msg`.
            getEthAddressStringFromBinary(
                context->delegatee,
                msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
                msg->pluginSharedRW->sha3,
                chainid
            );
            break;
    }
}

static void set_remove_delegate_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Delegate Address", msg->titleLength);
            
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';

            // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
            // Setting it to `0` will make it work with every chainID :)
            uint64_t chainid = 0;


            // Get the string representation of the address stored in `context->beneficiary`. Put it in
            // `msg->msg`.
            getEthAddressStringFromBinary(
                context->delegatee,
                msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
                msg->pluginSharedRW->sha3,
                chainid
            );
            break;
    }
}

static void set_reactivate_channel_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "ReActivate fee", msg->titleLength);
            
            copy_amount_with_ticker(context->channel_fees_amount,
                sizeof(context->channel_fees_amount),
                WEI_TO_ETHER,
                "DAI",
                sizeof("DAI"),
                msg->msg,
                msg->msgLength);

            break;
    }
}

static void set_deactivate_channel_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Tokens Received", msg->titleLength);

            copy_amount_with_ticker(context->tokens,
                sizeof(context->tokens),
                WEI_TO_ETHER,
                "PUSH",
                sizeof("PUSH"),
                msg->msg,
                msg->msgLength);

            break;
    }
}


static void set_create_channel_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Create Channel fee", msg->titleLength);
        
            copy_amount_with_ticker(context->channel_fees_amount,
                sizeof(context->channel_fees_amount),
                WEI_TO_ETHER,
                "DAI",
                sizeof("DAI"),
                msg->msg,
                msg->msgLength);

            break;
    }
}

static void set_approve_dai_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Beneficiary", msg->titleLength);
            
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';

            // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
            // Setting it to `0` will make it work with every chainID :)
            uint64_t chainid = 0;


            // Get the string representation of the address stored in `context->beneficiary`. Put it in
            // `msg->msg`.
            getEthAddressStringFromBinary(
                context->beneficiary,
                msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
                msg->pluginSharedRW->sha3,
                chainid
            );
            break;

        case 1:
            strlcpy(msg->title, "Approve Access", msg->titleLength);
            
            copy_amount_with_ticker(context->channel_fees_amount,
                sizeof(context->channel_fees_amount),
                WEI_TO_ETHER,
                "DAI",
                sizeof("DAI"),
                msg->msg,
                msg->msgLength);

            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case EPNS_ADD_DELEGATE:
            set_add_delegate_ui(msg, context);
            break;

        case EPNS_REMOVE_DELEGATE:
            set_remove_delegate_ui(msg, context);
            break;
        
        case EPNS_REACTIVATE_CHANNEL:
            set_reactivate_channel_ui(msg, context);
            break;

        case EPNS_DEACTIVATE_CHANNEL:
            set_deactivate_channel_ui(msg, context);
            break;
        
        case EPNS_CREATE_CHANNEL:
            set_create_channel_ui(msg, context);
            break;
        
        case EPNS_DAI_APPROVE:
            set_approve_dai_ui(msg, context);
            break;
        
        default:
            PRINTF("Received an invalid selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}